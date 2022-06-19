
#include "gobot_house.h"

void GobotHouse_2206::Setup(){
	this->__segments = 3;
	this->__map.Init();
	this->_gcode_queue = new GcodeQueue();

    Serial.print("\n[Debug] GobotHouse_2206::Setup() is done..........");
}

void GobotHouse_2206::SpinOnce(){
	// this->_gcode_queue->SpinOnce();
	if (!this->_gcode_queue->BufferIsFull())
		// My Input mq is from MQTT, My output mq is this->_gcode_queue.
		this->CheckMqttCommand();
}
void GobotHouse_2206::ExecuteMqttCommand(const char* command){
	this->_gcode_queue->AppendGcodeCommand(command);
}


void GobotHouse_2206::__Home(){
	bool via_inverse_kinematic = true;
	String strG28 = "G28B";
	if (via_inverse_kinematic) strG28.concat("I");
	this->_gcode_queue->AppendGcodeCommand(strG28);
	strG28 = "G28A";
	if (via_inverse_kinematic) strG28.concat("I");
	this->_gcode_queue->AppendGcodeCommand(strG28);
}
void GobotHouse_2206::Calibrate(int step, bool enable_eef_coil){

	if (step ==1){
		// STEPS_PER_ROUND_MOTOR, this variable should be calculated by electronic designer,
		// Here, we want to verify the value is correct.
		// Our idea is:
		//		Route the motor-shaft to run 10/20/50/100 rounds.
		String strG = "G1R-5F6000";
		this->_gcode_queue->AppendGcodeCommand(strG);
		strG = "G4S5";
		this->_gcode_queue->AppendGcodeCommand(strG);
		strG = "G1R0";
		this->_gcode_queue->AppendGcodeCommand(strG);
	}

	if (step==2){
		// config.Homed_position_alpha_in_degree, this variable should be calculated.
		// Our idea is:
		//		after home, move alpha actuator to zero angle, and PI angle.
		//      because it's easy to make a mark there.
		this->__Home();
		this->__Enable_eefCoil(enable_eef_coil);
		String strG = "G1A0";
		this->_gcode_queue->AppendGcodeCommand(strG);
		strG = "G4S5";
		this->_gcode_queue->AppendGcodeCommand(strG);
		strG = "G1A-180";
		this->_gcode_queue->AppendGcodeCommand(strG);
		strG = "G4S5";
		this->_gcode_queue->AppendGcodeCommand(strG);
		strG = "G1A0";
		this->_gcode_queue->AppendGcodeCommand(strG);
		this->__Enable_eefCoil(false);

	}
	if(step==3){
		//Continued from step2,  go on with:   config.Homed_position_beta_in_degree
		this->__Home();
		this->__Enable_eefCoil(enable_eef_coil);
		String strG = "G1A-180";
		this->_gcode_queue->AppendGcodeCommand(strG);
		strG = "G1B0";
		this->_gcode_queue->AppendGcodeCommand(strG);
		this->__Pause(5);
		strG = "G1B90";
		this->_gcode_queue->AppendGcodeCommand(strG);
		this->__Enable_eefCoil(false);
		this->__Pause(5);
		this->__Enable_eefCoil(enable_eef_coil);
		// this->ParkArms(false);
		this->__PreHome();
		this->__Enable_eefCoil(false);
	}
	if (step==4){
		// Calbrate Parking position with (alpha, beta)
		this->ParkArms(true);
		this->__Pause(5);
		this->__PreHome();
	}
	if (step==5){
		// #define ENDER_COIL_2109 32
		// #define ENDER_COIL_EXT_2109 33
		String strM = "M42 P33 S1";
		this->_gcode_queue->AppendGcodeCommand(strM);
		strM = "G4S5";
		this->_gcode_queue->AppendGcodeCommand(strM);
		strM = "M42 P33 S0";
		this->_gcode_queue->AppendGcodeCommand(strM);
	}
	if (step==6){
		this->__Home();
		for(int i=7; i>=0;i--){
			this->__Enable_eefCoil(enable_eef_coil);
			this->__Move_fromRoom_toDoor(i, false);
			this->__Enable_eefCoil(false);
			this->__Pause(1);
			this->__Enable_eefCoil(enable_eef_coil);
			this->__Move_fromRoom_toDoor(i, true);
		}
		this->__Enable_eefCoil(false);
		this->__PreHome();
	}
	if (step>=9){
		this->__Home();
		this->__Enable_eefCoil(enable_eef_coil);
		if (step == 9 ) 
			this->__Move_fromHead_toNeck(false);
		else
			this->__Move_fromParking_toDoor(step-10);
		this->__Enable_eefCoil(false);
		this->__Pause(5);
		this->__Enable_eefCoil(enable_eef_coil);
		if (step==9)
			this->__Move_fromHead_toNeck(true);
		else{
			this->__Move_fromRoom_toDoor(step-10,false);
			this->__Pause(5);
			this->__Move_fromRoom_toDoor(step-10,true);
			// this->ParkArms(false);
			this->__PreHome();
		}
		this->__Enable_eefCoil(false);
		this->__PreHome();
	}

	// while (true){
	// 	this->SpinOnce();
	// }
}

void GobotHouse_2206::ParkArms(bool do_homing){
	Serial.print("\n[Debug] GobotHouse_2206::ParkArms() is entering");
	int free_buffer_count = 0;
	while (free_buffer_count < 3){
		this->SpinOnce();
		free_buffer_count = this->_gcode_queue->GetFreeBuffersCount();
		Serial.print(free_buffer_count);
		Serial.print(" ");
	}

	if (do_homing){
		String strG28 = "G28B";
		this->_gcode_queue->AppendGcodeCommand(strG28);
		strG28 = "G28A";
		this->_gcode_queue->AppendGcodeCommand(strG28);
	}
	// this->_gcode_queue->SpinOnce();
	// Park Arms
	String strG1 = "G1B120 F2800";
	this->_gcode_queue->AppendGcodeCommand(strG1);
	strG1 = "G1A-60 F2800";
	this->_gcode_queue->AppendGcodeCommand(strG1);
}
// Head is a position name, The 5 bar arm will pick up stone from there.
void GobotHouse_2206::Test_MoveStone_FromRoomToHead(int loop_count, uint8_t room_id){
	// if (this->_gcode_queue->GetFreeBuffersCount() < 16)  return false;
	if (loop_count==0) return;

	for(int i=0; i<loop_count; i++){
		// __Move_fromNeck_toDoor(room_id,true);
		// __Move_fromRoom_toDoor(room_id,false);
		// __Enable_eefCoil(true);
		// __Move_fromRoom_toDoor(room_id, true);
		// __Move_fromNeck_toDoor(0, false);
		__Move_fromHead_toNeck(false);
		// __Enable_eefCoil(false);
		// __Move_fromHead_toNeck(true);
	}
}

void GobotHouse_2206::Test_MoveStone_FromHeadToRoom(int loop_count,uint8_t room_id){
	// if (this->_gcode_queue->GetFreeBuffersCount() < 16) return false;
	if (loop_count==0) return;
	for(int i=0; i<loop_count; i++){
		__Move_fromNeck_toDoor(0, false);  // 0 is useless.
		__Move_fromHead_toNeck(false);
		__Enable_eefCoil(true);
		__Move_fromHead_toNeck(true);
		__Move_fromNeck_toDoor(room_id, true);
		__Move_fromRoom_toDoor(room_id, false);
		__Enable_eefCoil(false);
		__Move_fromRoom_toDoor(room_id, true);
	}
}

void GobotHouse_2206::__Enable_eefCoil(bool enable){
	// String strM = "M42 P33 S1";
	// if (!enable) strM = "M42 P33 S0";
	String strM = "M280 P2S0";
	if (enable) strM = "M280 P2S128";
	this->_gcode_queue->AppendGcodeCommand(strM);

}

void GobotHouse_2206::__Move_fromHead_toNeck(bool forwarding){
	float x1 = this->__map.head.x;
	float x2 = this->__map.neck.x;
	if(!forwarding){
		//from neck to head
		x1 = this->__map.neck.x;
		x2 = this->__map.head.x;
	}
	float distance = x2 - x1;
	float dx = distance / this->__segments ;
	for(int segment= 0; segment <= this->__segments; segment++){
		float x = x1 + dx * segment;
		String gcode="G1X";
		String sx= String(x,2);
		gcode.concat(sx);
		gcode.concat("Y");
		gcode.concat(0);

		this->_gcode_queue->AppendGcodeCommand(gcode);
	}
}

void GobotHouse_2206::__Move_fromRoom_toDoor(uint8_t room_id, bool forwarding){
	//from room to door, now is at room.
	float x1 = __map.rooms[room_id].x;
	float y1 = __map.rooms[room_id].y;
	float x2 = __map.doors[room_id].x;
	float y2 = __map.doors[room_id].y;

	if (!forwarding){
		//from door to room, now is at door.
		x1 = __map.doors[room_id].x;
		y1 = __map.doors[room_id].y;
		x2 = __map.rooms[room_id].x;
		y2 = __map.rooms[room_id].y;
	}

	// float distance = __map.distance_room_to_door[room_id];
	float dx = (x2 - x1) / __segments;
	float dy = (y2 - y1) / __segments;
	for(int segment= 0; segment <= __segments; segment++){
		float x = x1 + dx * segment;
		float y = y1 + dy * segment;
		// MoveTo(x,y);
		String strGcode="G1X";
		strGcode.concat(x);
		strGcode.concat("Y");
		strGcode.concat(y);
		this->_gcode_queue->AppendGcodeCommand(strGcode);
	}
}

// This is almost a  rotation, because beta should be no changing.
void GobotHouse_2206::__Move_fromNeck_toDoor(uint8_t room_id, bool forwarding){
	Serial.print("[Debug] GobotHouse_2206::__Move_fromNeck_toDoor() room_id= ");
	Serial.print(room_id);

	float x = __map.doors[room_id].x;
	float y = __map.doors[room_id].y;
	Serial.println("111111111111");
	if (!forwarding){
		x = __map.neck.x;
		y = __map.neck.y;    
	}
	String strGcode="G1X";
	strGcode.concat(x);
	strGcode.concat("Y");
	strGcode.concat(y);
	Serial.println("2222222222222222");

	this->_gcode_queue->AppendGcodeCommand(strGcode);
	Serial.println("3333333333333333");

}
void GobotHouse_2206::__Move_fromParking_toDoor(uint8_t door_id){
	String strGcode="G1X";
	strGcode.concat(this->__map.doors[door_id].x);
	strGcode.concat("Y");
	strGcode.concat(this->__map.doors[door_id].y);
	Serial.println(strGcode);
	this->_gcode_queue->AppendGcodeCommand(strGcode);
}
void GobotHouse_2206::__Move_fromParking_toNeck(){
	String strGcode="G1X";
	strGcode.concat(this->__map.neck.x);
	strGcode.concat("Y");
	strGcode.concat(this->__map.neck.y);
	this->_gcode_queue->AppendGcodeCommand(strGcode);
}

void GobotHouse_2206::__Pause(uint8_t second){
		String strG1 = "G4S";
		strG1.concat(second);
		this->_gcode_queue->AppendGcodeCommand(strG1);
}

void GobotHouse_2206::__PreHome(){
	String strG1 = "G1B130";
	this->_gcode_queue->AppendGcodeCommand(strG1);
	strG1 = "G1A0";
	this->_gcode_queue->AppendGcodeCommand(strG1);
}


void GobotHouse_2206::Test_Alpha(int loop_count){
	if (loop_count == 0) return;

	Serial.println("[Info]  GobotMain::Test_HomeAlpha()");
	String g28 = "G28AI";
	String g1 = "G1A-20";
	bool buffer_is_full = false;
	for (int i=0; i<loop_count; i++){
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g28);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain::Test_HomeAlpha() Buffer is full, return");
			return;
		}
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g1);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain::Test_HomeAlpha() Buffer is full, return");
			return;
		}
	}
}

void GobotHouse_2206::Test_Beta(int loop_count){
	if (loop_count == 0) return;

	Serial.println("[Info]  GobotMain::Test_HomeBeta()  This is in TODO list.");
	String g28 = "G28BI";
	String g1 = "G1B120";
	String g4 = "G4S3";   //Wait for beta is left hall sensor.
	bool buffer_is_full = false;
	for (int i=0; i<loop_count; i++){
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g28);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain::Test_HomeBeta() Buffer is full, return");
			return;
		}
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g1);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain::Test_HomeBeta() Buffer is full, return");
			return;
		}
		this->_gcode_queue->AppendGcodeCommand(g4);
	}
}
