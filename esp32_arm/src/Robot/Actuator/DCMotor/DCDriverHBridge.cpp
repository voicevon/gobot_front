#include "DCDriverHBridge.h"

DCDriverHBridge::DCDriverHBridge(){
	}
	
void DCDriverHBridge::_Init(int pinA, int pinB){
	// Pin initialization
	_pinA = pinA;
	_pinB = pinB;
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
}

void DCDriverHBridge::Start(int pwm_speed, bool dir_forward){
	if (dir_forward){
		digitalWrite(_pinA, HIGH);
		digitalWrite(_pinB, LOW);
	}else{
		digitalWrite(_pinA, LOW);
		digitalWrite(_pinB, HIGH);
	}
}
void DCDriverHBridge::Stop(){
		digitalWrite(_pinA, LOW);
		digitalWrite(_pinB, LOW);

}
void DCDriverHBridge::Break(){
		digitalWrite(_pinA, HIGH);
		digitalWrite(_pinB, HIGH);
}
