#include "gobot_chessboard.h"
#include <HardwareSerial.h>
// #include "gobot_chessboard_hw.h"
// #include "CNC/cnc_five_bars/gobot_chessboard_hw.h"
#include "CNC/cnc_five_bars/cnc_five_bars.h"


void GobotChessboard::SpinOnce(){
	// this->_gcode_queue->SpinOnce();
	if (!this->_gcode_queue->BufferIsFull())
		// My Input mq is from MQTT, My output mq is this->_gcode_queue.
		this->CheckMqttCommand();
}
void GobotChessboard::ExecuteMqttCommand(const char* command){
	this->_gcode_queue->AppendGcodeCommand(command);
}

void GobotChessboard::ParkArms(bool do_home){
	Serial.print("\n[Debug] GobotChessboard::ParkArms() is entering");
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

void GobotChessboard::pick_place_park(RobotAction* pAction){
	uint8_t action_code = pAction->Arm.action_code;
		// for(int i=0;i<14;i++){
		//   Serial.print(pAction->bytes[i]);
		//   Serial.print(",  ");
		// }

	if ((action_code & (1<<1)) > 0){
		Serial.print("\npicking up "); 
		// MoveTo(pAction->Arm.pickup_x, pAction->Arm.pickup_y);
		// SetEffector(Lower);
		// SetEffector(Suck);
		// SetEffector(Higher);
	}
	if ((action_code & (1<<2)) > 0){
		Serial.print("\n  placing     ");
		// for(int i=5;i<9;i++){
		//   Serial.print(pAction->bytes[i]);
		//   Serial.print(",  ");
		// }    
		// MoveTo(pAction->Arm.place_x, pAction->Arm.place_y);
		// SetEffector(Lower);
		// SetEffector(Release);
		// SetEffector(Higher);
		// SetEffector(Sleep);
	}
	if ((action_code & (1<<3)) > 0){
		Serial.print("\n  parking     ");
		// MoveTo(pAction->Arm.park_x, pAction->Arm.park_y);
		// SetEffector(Sleep);
	}
	// pAction->bytes[0] = 1;
}

String GobotChessboard::__GetGcode_for_eef_action(EefAction eef_action){
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

void GobotChessboard::__Pickup(ChessboardCell* cell){
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

void GobotChessboard::__Place(ChessboardCell* cell){
	// Move to that cell, Lower, Unload, Up, Sleep

}

void GobotChessboard::__Park(){
	// Move to park point.
	ChessboardCell park_point = ChessboardCell('W',0);
	String gc = park_point.GetG1Code();
	this->_gcode_queue->AppendGcodeCommand(gc);
}

void GobotChessboard::__Home(){
	String g = "G28AI";
	this->_gcode_queue->AppendGcodeCommand(g);
	g = "G28BI";
	this->_gcode_queue->AppendGcodeCommand(g);

}

void GobotChessboard::__Calibrate_99(){
	this->__Home();
	for (uint8_t x =0; x<=19;x++){
		for (uint8_t y=0; y<=19; y++){
			ChessboardCell cell = ChessboardCell(x,y);
			String gc = cell.GetG1Code();
			this->_gcode_queue->AppendGcodeCommand(gc);
		}
	}
}

void GobotChessboard::Calibrate(int step){
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
