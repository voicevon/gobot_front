#include "gobot_chessboard.h"
#include <HardwareSerial.h>
#include "gobot_chessboard_hw.h"


void GobotChessboard::Init(){
	Serial.print("\n[Info] GobotChessboard::Init() is entering.");
	GobotChessboardHardware* objGobotHardware = new GobotChessboardHardware();
	objGobotHardware->Init();
	this->__commandQueue = new CommandQueue();
	this->__commandQueue->LinkRobot(objGobotHardware);
	Serial.print("\n[Info] GobotChessboard::Init() is done.");

}

void GobotChessboard::SpinOnce(){
	this->__commandQueue->SpinOnce();
}

void GobotChessboard::ParkArms(bool do_home){
	Serial.print("\n[Debug] GobotChessboard::ParkArms() is entering");
	if (do_home){
		String strG28 = "G28A";
		this->__commandQueue->AppendGcodeCommand(strG28);
		strG28 = "G28B";
		this->__commandQueue->AppendGcodeCommand(strG28);
	}
	// Park Arms
	String strG1 = "G1B0 F2800";
	this->__commandQueue->AppendGcodeCommand(strG1);
	strG1 = "G1A-180 F2800";
	this->__commandQueue->AppendGcodeCommand(strG1);
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
