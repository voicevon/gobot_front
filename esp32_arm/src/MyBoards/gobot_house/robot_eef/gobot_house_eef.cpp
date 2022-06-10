#include "gobot_house_eef.h"

RobotEef_GobotHouse::RobotEef_GobotHouse(){
	
}

void RobotEef_GobotHouse::Init(){
	pinMode(PIN_EEF_COIL_A_2109, OUTPUT);
    pinMode(PIN_EEF_COIL_B_2109, OUTPUT);

	this->__eefServo.setPeriodHertz(50);
	this->__eefServo.attach(PIN_EEF_SERVO);
}


void RobotEef_GobotHouse::Run(uint8_t eef_code) {
	switch (eef_code){
		case EEF_CODE_DOWN:
			this->__eefServo.write(180);
			break;
		case EEF_CODE_UP:
			this->__eefServo.write(0);
			break;
		case EEF_CODE_LOAD:
			digitalWrite(PIN_EEF_COIL_A_2109, HIGH);
			digitalWrite(PIN_EEF_COIL_B_2109, LOW);
			break;
		case EEF_CODE_UNLOAD:
			digitalWrite(PIN_EEF_COIL_A_2109,LOW);
			digitalWrite(PIN_EEF_COIL_B_2109,HIGH);
			break;
		case EEF_CODE_SLEEP:
			digitalWrite(PIN_EEF_COIL_A_2109,LOW);
			digitalWrite(PIN_EEF_COIL_B_2109,LOW);
			break;
		default:
			Serial.print("[Warn]  RobotEef_GobotHouse::Run()  Unknown EEF_CODE = ");
			Serial.println(eef_code);
			break;
	}
}