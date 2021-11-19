#include "h_bridge_l298n.h"

// int dutyCycle = 200;

void L298N::Init(uint8_t pwm_channel, uint8_t pin_enable, uint8_t pinA, uint8_t pinB){
	this->_pinA = pinA;
	this->_pinB = pinB;
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
    
    this->_pwm_channel = pwm_channel;
    this->_pin_enable = pin_enable;
    pinMode(pin_enable, OUTPUT);
    ledcAttachPin(pin_enable, pwm_channel); // attach the channel to the GPIO to be controlled
    ledcWrite(pwm_channel, 0); 
}

void L298N::MoveAtSpeed(int pwm_speed, bool dir_forward){
    if (dir_forward){
        digitalWrite(_pinA, HIGH);
        digitalWrite(_pinB, LOW);
    }else{
        digitalWrite(_pinA, LOW);
        digitalWrite(_pinB, HIGH);
    }
    // digitalWrite(this->_pin_enable, HIGH);
    ledcWrite(this->_pwm_channel, pwm_speed); 

}
void L298N::Stop(){
    digitalWrite(_pinA, LOW);
    digitalWrite(_pinB, LOW);
    ledcWrite(this->_pwm_channel, 0); 

} 

void L298N::Break(){
    //TODO:: Read speed from encoder, while  abs (current_speed > 5), move inverse direction.
    digitalWrite(this->_pin_enable, LOW);
} 
