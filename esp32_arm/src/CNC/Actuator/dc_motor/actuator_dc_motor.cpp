#include "actuator_dc_motor.h"
#include "Arduino.h"

#define PWM1_Ch0 0
#define PWM1_Ch1 1
#define PWM1_Res 8
#define PWM1_Freq 1000

ActuatorDcMotor::ActuatorDcMotor(uint8_t h_bridge_pin_a, uint8_t h_bridge_pin_b){
    pinMode(h_bridge_pin_a, OUTPUT);
    pinMode(h_bridge_pin_b, OUTPUT);
    digitalWrite(h_bridge_pin_a, LOW);
    digitalWrite(h_bridge_pin_b, LOW);
    // assign ledc channel

    // init ledc 
 
    this->__h_bridge_pin_a = h_bridge_pin_a;
    this->__h_bridge_pin_b = h_bridge_pin_b;


    ledcAttachPin (__h_bridge_pin_a, PWM1_Ch0);
	ledcAttachPin (__h_bridge_pin_b, PWM1_Ch1);
	ledcSetup (PWM1_Ch0, PWM1_Freq, PWM1_Res);
	ledcSetup (PWM1_Ch1, PWM1_Freq, PWM1_Res);
}

void ActuatorDcMotor::SpinOnce(){
    // real speed control, position check, auto stop....

    if(this->GetDistanceToTarget_InCncUnit() == 0){
        this->Stop();
    }else{
        // control speed
        float error = this->__sensor->getVelocity() - this->__speed; 
        float new_speed =  0;   //   pid.get_speed(error);
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

float ActuatorDcMotor::GetDistanceToTarget_InCncUnit(){
    // sensor --> current poistion   --> distance to target
    return this->_target_cnc_position - this->GetCurrentPosition_InCncUnit();

}


void ActuatorDcMotor::UpdateSpeedWhenMotorIsRunning(float new_speed){
        this->__speed = new_speed;
        this->StartToMove();
}

void ActuatorDcMotor::SetSpeed(float speed_per_second){
    // set the motor speed 
    this->__speed = speed_per_second;

}

void ActuatorDcMotor::Stop(){
    // motor A and motor B setted LOW to stop DC motor
    digitalWrite(__h_bridge_pin_a, LOW);
    digitalWrite(__h_bridge_pin_b, LOW);
}



void ActuatorDcMotor::StartToMove(){
    // Get direction from   this->_target_cnc_position    this->GetCurrentPosition_InCncUnit()
     
    bool dir_is_cw = (this->_target_cnc_position - this->GetCurrentPosition_InCncUnit()) > 0;

    // this function be used to start DC motor(CW and CCW) 
    // float Speed=128;
    // make  DCmotor CW 
    // if(/* PIN_TOUCH_UP == LOW      */){
    //     // ledcWrite (PWM1_Ch0,Speed);
    //     ledcWrite (PWM1_Ch0, this->__speed);
    //     digitalWrite (__h_bridge_pin_b,LOW);
    // }
    // // make  DCmotor CCW 
    // else if(/*PIN_TOUCH_DOWN ==LOW     */){
    //     ledcWrite (PWM1_Ch1, this->__speed);
    //     digitalWrite(__h_bridge_pin_a,LOW);
    // }
}
