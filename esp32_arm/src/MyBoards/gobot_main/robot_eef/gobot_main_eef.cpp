#include "gobot_main_eef.h"

RobotEef_GobotMain::RobotEef_GobotMain(){
	
}

void RobotEef_GobotMain::Run(uint8_t eef_code) {
	switch (eef_code){
		// case EEF_CODE_DOWN:
		// 	this->eefServo->write(180);
		// 	break;
		// case EEF_CODE_UP:
		// 	this->eefServo->write(0);
			// break;
		case EEF_CODE_LOAD:
			digitalWrite(PIN_EEF_A_2201, HIGH);
			digitalWrite(PIN_EEF_B_2201, LOW);
			break;
		case EEF_CODE_UNLOAD:
			digitalWrite(PIN_EEF_A_2201,LOW);
			digitalWrite(PIN_EEF_B_2201,HIGH);
			break;
		case EEF_CODE_SLEEP:
			digitalWrite(PIN_EEF_A_2201,LOW);
			digitalWrite(PIN_EEF_B_2201,LOW);
			break;
		default:
			break;
	}
}