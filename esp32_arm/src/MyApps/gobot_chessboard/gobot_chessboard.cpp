#include "gobot_chessboard.h"


void GobotChessboard::Init(){
	GobotChessboardHardware* objGobotHardware = new GobotChessboardHardware();
	objGobotHardware->Init();
	this->__commandQueue->LinkRobot(objGobotHardware);

}

void GobotChessboard::SpinOnce(){
	this->__commandQueue->SpinOnce();
}

void GobotChessboard::ParkArms(bool do_home){

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
