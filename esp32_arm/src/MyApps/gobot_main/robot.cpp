#include "robot.h"
#include <HardwareSerial.h>
#include "CNC/arm_solution/five_bars/cnc_five_bars_base.h"


void GobotMain_Robot::SpinOnce(){
	if (!this->_gcode_queue->BufferIsFull())
		// My Input mq is from MQTT, My output mq is this->_gcode_queue.
		this->CheckMqttCommand();
}
void GobotMain_Robot::AsyncExecuteMqttCommand(const char* command){
	this->_gcode_queue->AppendGcodeCommand(command);
}

void GobotMain_Robot::ParkArms(bool do_home){
	Serial.print("\n[Debug] GobotMain_Robot::ParkArms() is entering");
	if (do_home){
		String strG28 = "G28AI";
		this->_gcode_queue->AppendGcodeCommand(strG28);
		strG28 = "G28BI";
		this->_gcode_queue->AppendGcodeCommand(strG28);
	}
	// Park Arms
	String strG1 = "G1B0 F2800";
	this->_gcode_queue->AppendGcodeCommand(strG1);
	strG1 = "G1A-180 F2800";
	this->_gcode_queue->AppendGcodeCommand(strG1);
}

String GobotMain_Robot::__GetGcode_for_eef_action(EefAction eef_action){
	switch (eef_action)
	{
	case EefAction::Sleep:
		/* code */
		break;

	case EefAction::Load:
		break;

	case EefAction::Unload:
		break;

	case EefAction::Lower:
		break;

	case EefAction::Higher:
		break;


	default:
		break;
	}
	String xx = "";
	return xx;
}

void GobotMain_Robot::__Pickup(ChessboardCell* cell){
	// Move to that cell, Lower, Load stone, Up
	String gc = cell->GetG1Code();
	this->_gcode_queue->AppendGcodeCommand(gc);
	gc = this->__GetGcode_for_eef_action(EefAction::Lower);
	this->_gcode_queue->AppendGcodeCommand(gc);
	gc = this->__GetGcode_for_eef_action(EefAction::Load);
	this->_gcode_queue->AppendGcodeCommand(gc);
	gc = this->__GetGcode_for_eef_action(EefAction::Higher);
	this->_gcode_queue->AppendGcodeCommand(gc);

}

void GobotMain_Robot::__Place(ChessboardCell* cell){
	// Move to that cell, Lower, Unload, Up, Sleep

}

void GobotMain_Robot::__Park(){
	// Move to park point.
	ChessboardCell park_point = ChessboardCell('W',0);
	String gc = park_point.GetG1Code();
	this->_gcode_queue->AppendGcodeCommand(gc);
}

void GobotMain_Robot::__Home(){
	String g = "G28AI";
	this->_gcode_queue->AppendGcodeCommand(g);
	g = "G28BI";
	this->_gcode_queue->AppendGcodeCommand(g);

}

void GobotMain_Robot::__Calibrate_99(){
	this->__Home();
	for (uint8_t x =0; x<=19;x++){
		for (uint8_t y=0; y<=19; y++){
			ChessboardCell cell = ChessboardCell(x,y);
			String gc = cell.GetG1Code();
			this->_gcode_queue->AppendGcodeCommand(gc);
		}
	}
}

void GobotMain_Robot::Calibrate(int step){
	if(step == 1){
		// Goal:
		//		To calibrate home_position_alpha.
		//		Please pay attention:  The most outside angle value is 180 in degree 
		// Process:
		// 		1, Home alpha motor, 
		// 		2, Then move to 180 degree , stand by for 5 seconds for observer.
		// 		3, Then move to 90 degree 
		String g = "G28AI";
		this->_gcode_queue->AppendGcodeCommand(g);
		g = "G1A180";
		this->_gcode_queue->AppendGcodeCommand(g);
		g = "G4S5";
		this->_gcode_queue->AppendGcodeCommand(g);
		g = "G1A90";
		this->_gcode_queue->AppendGcodeCommand(g);
	}
	if (step == 2){
		// Goal:
		//		To calibrate home_position_beta.
		//		Please pay attention:  The most outside angle value is 0 in degree 
		// Process:
		// 		1, Home beta motor, 
		// 		2, Then move to 0 degree , stand by for 5 seconds for observer.
		// 		3, Then move to 90 degree 
		String g = "G28BI";
		this->_gcode_queue->AppendGcodeCommand(g);
		g = "G1B0";
		this->_gcode_queue->AppendGcodeCommand(g);
		g = "G4S5";
		this->_gcode_queue->AppendGcodeCommand(g);
		g = "G1B180";
		this->_gcode_queue->AppendGcodeCommand(g);
	}
	if (step == 3){
		// Move to Park point.
		this->__Home();
		this->__Park();
	}
	if (step == 99){
		// Demo movement in a loop
		this->__Calibrate_99();
	}
}

void GobotMain_Robot::Test_HomeAlpha(int loop_count){
	if (loop_count == 0) return;

	Serial.println("[Info]  GobotMain_Robot::Test_HomeAlpha()");
	String g28 = "G28AI";
	String g1 = "G1A135";
	bool buffer_is_full = false;
	for (int i=0; i<loop_count; i++){
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g28);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain_Robot::Test_HomeAlpha() Buffer is full, return");
			return;
		}
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g1);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain_Robot::Test_HomeAlpha() Buffer is full, return");
			return;
		}
	}
}

void GobotMain_Robot::Test_HomeBeta(int loop_count){
	if (loop_count == 0) return;

	Logger::Info("GobotMain_Robot::Test_HomeBeta()");
	String g28 = "G28BI";
	String g1 = "G1B45";
	bool buffer_is_full = false;
	for (int i=0; i<loop_count; i++){
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g28);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain_Robot::Test_HomeBeta() Buffer is full, return");
			return;
		}
		buffer_is_full = this->_gcode_queue->AppendGcodeCommand(g1);
		if (buffer_is_full){
			Serial.println("[Warn] GobotMain_Robot::Test_HomeBeta() Buffer is full, return");
			return;
		}
	}
}

void GobotMain_Robot::Test_PickPlace(int loop_count){

}





