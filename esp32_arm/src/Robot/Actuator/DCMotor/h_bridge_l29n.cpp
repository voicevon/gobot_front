#include "h_bridge_l298n.h"
// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;   // so max pwm speed is 255
int dutyCycle = 200;


void L298N::Init(uint8_t pin_enable, uint8_t pinA, uint8_t pinB){
	this->_pinA = pinA;
	this->_pinB = pinB;
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
    
    this->_pin_enable = pin_enable;
    pinMode(pin_enable, OUTPUT);
    ledcSetup(pwmChannel, freq, resolution); // configure LED PWM functionalitites
    ledcAttachPin(pin_enable, pwmChannel); // attach the channel to the GPIO to be controlled
    ledcWrite(pwmChannel, 0); 
}

void L298N::Start(int pwm_speed, bool dir_forward){
    if (dir_forward){
        digitalWrite(_pinA, HIGH);
        digitalWrite(_pinB, LOW);
    }else{
        digitalWrite(_pinA, LOW);
        digitalWrite(_pinB, HIGH);
    }
    // digitalWrite(this->_pin_enable, HIGH);
    ledcWrite(pwmChannel, pwm_speed); 

}
void L298N::Stop(){
    digitalWrite(_pinA, LOW);
    digitalWrite(_pinB, LOW);
    ledcWrite(pwmChannel, 0); 

} 

void L298N::Break(){
    //TODO:: Read speed from encoder, while  abs (current_speed > 5), move inverse direction.
    digitalWrite(this->_pin_enable, LOW);
} 
