#include "gobot_house_eef.h"

RobotEef_GobotHouse::RobotEef_GobotHouse(){
	
}

void RobotEef_GobotHouse::Init(){
	// pinMode(PIN_EEF_COIL_A_2109, OUTPUT);
    // pinMode(PIN_EEF_COIL_B_2109, OUTPUT);

	this->__eefServo.setPeriodHertz(50);
	this->__eefServo.attach(PIN_EEF_SERVO);
}

#define LOAD_ANGLE 98
#define UNLOAD_ANGLE LOAD_ANGLE + 70

void RobotEef_GobotHouse::Run(uint8_t eef_code) {
	Serial.print("[Debug] RobotEef_GobotHouse::Run() eef_code= ");
	Serial.println(eef_code);
	
	switch (eef_code){
		case EEF_CODE_LOAD:
			this->__eefServo.write(LOAD_ANGLE);
			break;
		case EEF_CODE_UNLOAD:
			this->__eefServo.write(UNLOAD_ANGLE);
			break;
			break;
		default:
			Serial.print("[Warn]  RobotEef_GobotHouse::Run()  Unknown EEF_CODE = ");
			Serial.println(eef_code);
			break;
	}
}