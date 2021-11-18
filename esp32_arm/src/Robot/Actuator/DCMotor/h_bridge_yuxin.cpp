#include "h_bridge_yuxin.h"

void YuXin_HBridge::Init(uint8_t pinA, uint8_t pinB){
	// Pin initialization
	_pinA = pinA;
	_pinB = pinB;
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
}



void YuXin_HBridge::Start(int pwm_speed, bool dir_forward){
    if (dir_forward){
        digitalWrite(_pinA, HIGH);
        digitalWrite(_pinB, LOW);
    }else{
        digitalWrite(_pinA, LOW);
        digitalWrite(_pinB, HIGH);
    }
}
void YuXin_HBridge::Stop(){
} 

void YuXin_HBridge::Break(){
    //TODO:: Read speed from encoder, while  abs (current_speed > 5), move inverse direction.
} 
