#include "h_bridge_l298n.h"

void L298N::Init(uint8_t pin_enable, uint8_t pin_a, uint8_t pin_b){
    this->_Init(pin_a, pin_b);
    this->_pin_enable = pin_enable;
    pinMode(pin_enable, OUTPUT);
    digitalWrite(pin_enable, LOW);
}

void L298N::Start(int pwm_speed, bool dir_forward){
    if (dir_forward){
        digitalWrite(_pinA, HIGH);
        digitalWrite(_pinB, LOW);
    }else{
        digitalWrite(_pinA, LOW);
        digitalWrite(_pinB, HIGH);
    }
    digitalWrite(this->_pin_enable, HIGH);
}
void L298N::Stop(){
    digitalWrite(this->_pin_enable, LOW);
} 

void L298N::Break(){
    //TODO:: Read speed from encoder, while  abs (current_speed > 5), move inverse direction.
    digitalWrite(this->_pin_enable, LOW);
} 
