#include "h_bridge_yuxin.h"

void YuXin_HBridge::Init(uint8_t pin_a, uint8_t pin_b){
    this->_Init(pin_a, pin_b);
    // this->_pin_enable = pin_enable;
    // pinMode(pin_enable, OUTPUT);
    // digitalWrite(pin_enable, LOW);
}

void YuXin_HBridge::Start(int pwm_speed, bool dir_forward){
    if (dir_forward){
        digitalWrite(_pinA, HIGH);
        digitalWrite(_pinB, LOW);
    }else{
        digitalWrite(_pinA, LOW);
        digitalWrite(_pinB, HIGH);
    }
    // digitalWrite(this->_pin_enable, HIGH);
}
void YuXin_HBridge::Stop(){
    // digitalWrite(this->_pin_enable, LOW);
} 

void YuXin_HBridge::Break(){
    //TODO:: Read speed from encoder, while  abs (current_speed > 5), move inverse direction.
    // digitalWrite(this->_pin_enable, LOW);
} 
