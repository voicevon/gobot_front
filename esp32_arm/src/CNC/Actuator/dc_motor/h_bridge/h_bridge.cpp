#include "h_bridge.h"
#include <Arduino.h>
#include "MyLibs/board/board_base.h"

#define PWM_RESOLUTION 8
#define PWM_FREQUENCY 20

H_Bridge::H_Bridge(uint8_t h_bridge_pin_dir, uint8_t h_bridge_pin_pwm_speed){
    Init(h_bridge_pin_dir, h_bridge_pin_pwm_speed);
}

void H_Bridge::Init(uint8_t h_bridge_pin_dir, uint8_t h_bridge_pin_pwm_speed){
        this->__h_bridge_pin_dir = h_bridge_pin_dir;
    this->__h_bridge_pin_pwm_speed = h_bridge_pin_pwm_speed;

    pinMode(h_bridge_pin_dir, OUTPUT);
    pinMode(h_bridge_pin_pwm_speed, OUTPUT);
    digitalWrite(h_bridge_pin_dir, LOW);

    // init ledc via assign ledc channel
    BoardBase board;    //TODO:  double check , Is any risk ?

    this->__pwm_channel = board.Assign_ledc_channel();
	ledcAttachPin (h_bridge_pin_pwm_speed, this->__pwm_channel);
	ledcSetup (this->__pwm_channel, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcWrite(this->__pwm_channel, 0);
}



void H_Bridge::Stop(){
    // motor A and motor B setted LOW to stop DC motor
    digitalWrite(this->__h_bridge_pin_dir, LOW);
    ledcWrite(this->__pwm_channel, 0);
}

void H_Bridge::SetPwmSpeed(bool dir_is_cw,  uint32_t pwm_speed){
    if(dir_is_cw){
        digitalWrite(this->__h_bridge_pin_dir, LOW);
        ledcWrite(this->__pwm_channel, pwm_speed);
    } else {
        // make  DCmotor CCW 
        digitalWrite(this->__h_bridge_pin_dir, HIGH);
        ledcWrite(this->__pwm_channel, (256 - pwm_speed));
    }
}
