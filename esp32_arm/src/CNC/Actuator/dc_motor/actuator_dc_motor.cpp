#include "actuator_dc_motor.h"
#include "Arduino.h"
#include "MyBoards/board_base.h"

// #define PWM1_Ch0 0
// #define PWM1_Ch1 1
#define PWM1_Res 8
#define PWM1_Freq 1000
#define GEAR_TEETH_COUNT 56.0f
#define GEAR_PITCH 12.7f   //unit is mm
#define PID_P 1.0f
#define INERTIA_DISTANCE_IN_MM  50   // ??  in_rad


ActuatorDcMotor::ActuatorDcMotor(uint8_t h_bridge_pin_a, uint8_t h_bridge_pin_b){
    pinMode(h_bridge_pin_a, OUTPUT);
    pinMode(h_bridge_pin_b, OUTPUT);
    digitalWrite(h_bridge_pin_a, LOW);
    digitalWrite(h_bridge_pin_b, LOW);

    // init ledc via assign ledc channel
    BoardBase board;
    this->__pwm_channel_a = board.Assign_ledc_channel();
    ledcAttachPin (__h_bridge_pin_a, this->__pwm_channel_a);
	ledcSetup (this->__pwm_channel_a, PWM1_Freq, PWM1_Res);

    this->__pwm_channel_b = board.Assign_ledc_channel();
	ledcAttachPin (__h_bridge_pin_b, this->__pwm_channel_b);
	ledcSetup (this->__pwm_channel_b, PWM1_Freq, PWM1_Res);

    this->__h_bridge_pin_a = h_bridge_pin_a;
    this->__h_bridge_pin_b = h_bridge_pin_b;


    // Calculate rad_per_mm,  This is determined by mechanic designer.
    this->__sensor_rad_per_mm = GEAR_PITCH * GEAR_TEETH_COUNT / TWO_PI; 
    this->__offset = 0;
}

// void ActuatorDcMotor::TestDriver(bool dir_is_cw, int pwm_speed){
//     this->__pwm_speed = pwm_speed;
//     this->
// }


void ActuatorDcMotor::SpinOnce(){
    // real speed control, position check, auto stop....

    if(this->GetAbsDistanceToTarget_InCncUnit() < INERTIA_DISTANCE_IN_MM){
        // The wheel will continue to run a short time, because the inertia
        // TDDO:  How to deal with negtive distance?
        this->Stop();
    }else{
        // control speed
        float error = this->__sensor->getVelocity() - this->__cnc_speed; 
        float new_speed = - PID_P * error;   //   pid.get_speed(error);
        this->UpdateSpeedWhenMotorIsRunning(new_speed);
    }
}

float ActuatorDcMotor::GetCurrentPosition_InCncUnit(){
    float current_postion_in_mm = this->__sensor_rad_per_mm * this->__sensor->getAngle() + this->__offset;
    return current_postion_in_mm;
}

void ActuatorDcMotor::SetCurrentPositionAs(float position_in_cnc_unit){
    // known:    current angle,     position_in_cnc_unit
    this->__offset = position_in_cnc_unit - this->__sensor->getAngle() * this->__sensor_rad_per_mm;
    // the radius of wheel is not sure
    // 
    // if( (position_in_cnc_unit/5) < this->__sensor->getAngle()){//   ??
    //     this->Stop();
    // }
}

void ActuatorDcMotor::SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit){
    if (is_absolute_position){
        this->_target_cnc_position = position_in_cnc_unit;
    }else{
        this->_target_cnc_position = this->GetCurrentPosition_InCncUnit() + position_in_cnc_unit;
    }
}

float ActuatorDcMotor::GetAbsDistanceToTarget_InCncUnit(){
    // sensor --> current poistion   --> distance to target
    // TODO:  minus distance.
    return this->_target_cnc_position - this->GetCurrentPosition_InCncUnit();
}

void ActuatorDcMotor::UpdateSpeedWhenMotorIsRunning(float new_cnc_speed){
        this->__pwm_speed = 12.23f * new_cnc_speed;   // todo:   ax^3 + bx^2 + cx + d
        bool dir_is_cw = (this->_target_cnc_position - this->GetCurrentPosition_InCncUnit()) > 0;
        this->StartToMove(dir_is_cw, this->__pwm_speed);
}

void ActuatorDcMotor::SetSpeed(float rad_per_second){
    this->__cnc_speed = rad_per_second;
}

void ActuatorDcMotor::SetAccelleration(float accelleration_in_cnc_unit){
    // this is a future feature.
}

void ActuatorDcMotor::Stop(){
    // motor A and motor B setted LOW to stop DC motor
    digitalWrite(__h_bridge_pin_a, LOW);
    digitalWrite(__h_bridge_pin_b, LOW);
}

void ActuatorDcMotor::StartToMove(bool dir_is_cw,  int pwm_speed){
    if(dir_is_cw){
        ledcWrite (this->__pwm_channel_a, pwm_speed);
        digitalWrite (this->__h_bridge_pin_b,LOW);
    }
    else {
        // make  DCmotor CCW 
        ledcWrite (this->__pwm_channel_b, pwm_speed);
        digitalWrite(this->__h_bridge_pin_a, LOW);
    }
}
