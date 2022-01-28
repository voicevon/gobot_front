
#include "gobot_house.h"
#include "Arduino.h"





void GobotHouse::Setup(RobotAction* pAction){
	this->__house_action = pAction;
	__segments = 5;
	__map.setup();

	this->__robot_hardware = &GobotHouseHardware::getInstance();
    this->__robot_hardware->Init_Linkage();
	this->__commandQueue = new CommandQueue();
	this->__commandQueue->LinkRobot(this->__robot_hardware);
    Serial.print("\n[Debug] GobotHouse::Setup() is done..........");
}

void GobotHouse::SpinOnce(){
	this->__robot_hardware->SpinOnce();
	this->__commandQueue->SpinOnce();
}
void GobotHouse::ParkArms(bool do_homing){
	Serial.print("\n[Debug] GobotHouse::ParkArms() is entering");
	int free_buffer_count = 0;
	while (free_buffer_count < 3){
		this->SpinOnce();
		free_buffer_count = this->__commandQueue->GetFreeBuffersCount();
		Serial.print(free_buffer_count);
		Serial.print(" ");
	}

	if (do_homing){
		String strG28 = "G28B";
		this->__commandQueue->AppendGcodeCommand(strG28);
		strG28 = "G28A";
		this->__commandQueue->AppendGcodeCommand(strG28);
	}
	this->__commandQueue->SpinOnce();
	String strG1 = "G1A-3.14 F2800";
	this->__commandQueue->AppendGcodeCommand(strG1);
}
// Head is a position name, The 5 bar arm will pick up stone from there.
bool GobotHouse::MoveStone_FromRoomToHead(uint8_t room_id){
	if (this->__commandQueue->GetFreeBuffersCount() < 16)  return false;

	__Move_fromNeck_toDoor(room_id,true);
	__Move_fromRoom_toDoor(room_id,false);
	__Enable_eefCoil(true);
	__Move_fromRoom_toDoor(room_id, true);
	__Move_fromNeck_toDoor(0, false);
	__Move_fromHead_toNeck(false);
	__Enable_eefCoil(false);
	__Move_fromHead_toNeck(true);
	return true;
}

bool GobotHouse::MoveStone_FromHeadToRoom(uint8_t room_id){
	if (this->__commandQueue->GetFreeBuffersCount() < 16) return false;
	__Move_fromNeck_toDoor(0, false);  // 0 is useless.
	__Move_fromHead_toNeck(false);
	__Enable_eefCoil(true);
	__Move_fromHead_toNeck(true);
	__Move_fromNeck_toDoor(room_id, true);
	__Move_fromRoom_toDoor(room_id, false);
	__Enable_eefCoil(false);
	__Move_fromRoom_toDoor(room_id, true);
	return true;
}

void GobotHouse::__Enable_eefCoil(bool enable){

}

void GobotHouse::__Move_fromHead_toNeck(bool forwarding){
	float x1 = this->__map.head.x;
	float x2 = this->__map.neck.x;
	if(!forwarding){
		//from neck to head
		x1 = this->__map.neck.x;
		x2 = this->__map.head.x;
	}
	float distance = x2 - x1;
	float dx = distance / this->__segments ;
	for(int segment= 0; segment < this->__segments; segment++){
		float x = x1 + dx * segment;
		String gcode="G1X";
		String sx= String(x,2);
		gcode.concat(sx);
		gcode.concat("Y");
		gcode.concat(0);

		this->__commandQueue->AppendGcodeCommand(gcode);
	}
}

void GobotHouse::__Move_fromRoom_toDoor(uint8_t room_id, bool forwarding){
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
	for(int segment= 0; segment < __segments; segment++){
		float x = x1 + dx * segment;
		float y = y1 + dy * segment;
		// MoveTo(x,y);
		String strGcode="G1X";
		strGcode.concat(x);
		strGcode.concat("Y");
		strGcode.concat(y);
		this->__commandQueue->AppendGcodeCommand(strGcode);
	}
}

// This is almost a  rotation, because beta should be no changing.
void GobotHouse::__Move_fromNeck_toDoor(uint8_t room_id, bool forwarding){
	float x = __map.doors[room_id].x;
	float y = __map.doors[room_id].y;
	if (!forwarding){
		x = __map.neck.x;
		y = __map.neck.y;    
	}
	String strGcode="G1X";
	strGcode.concat(x);
	strGcode.concat("Y");
	strGcode.concat(y);
	this->__commandQueue->AppendGcodeCommand(strGcode);
}