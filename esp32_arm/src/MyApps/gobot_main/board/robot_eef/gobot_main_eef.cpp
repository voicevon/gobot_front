#include "gobot_main_eef.h"

RobotEef_GobotMain::RobotEef_GobotMain(){
	
}

void RobotEef_GobotMain::Init(){
	pinMode(PIN_EEF_COIL_A_2201, OUTPUT);
    pinMode(PIN_EEF_COIL_B_2201, OUTPUT);

	this->__eefServo.setPeriodHertz(50);
	this->__eefServo.attach(PIN_EEF_SERVO_2201);
}

void RobotEef_GobotMain::Run(uint8_t eef_code) {
	switch (eef_code){
		case EEF_CODE_DOWN:
			this->__eefServo.write(20);
			break;
		case EEF_CODE_UP:
			this->__eefServo.write(180);
			break;
		case EEF_CODE_LOAD:
			digitalWrite(PIN_EEF_COIL_A_2201, HIGH);
			digitalWrite(PIN_EEF_COIL_B_2201, LOW);
			break;
		case EEF_CODE_UNLOAD:
			digitalWrite(PIN_EEF_COIL_A_2201,LOW);
			digitalWrite(PIN_EEF_COIL_B_2201,HIGH);
			break;
		case EEF_CODE_SLEEP:
			digitalWrite(PIN_EEF_COIL_A_2201,LOW);
			digitalWrite(PIN_EEF_COIL_B_2201,LOW);
			break;
		default:
			Serial.print("[Warn] RobotEef_GobotMain::Run() Unknown eef_code = ");
			Serial.println(eef_code);
			break;
	}
}