#include "actuator_dc_motor.h"
#include "Arduino.h"

ActuatorDcMotor::ActuatorDcMotor(uint8_t h_bridge_pin_a, uint8_t h_bridge_pin_b){
    pinMode(h_bridge_pin_a, OUTPUT);
    pinMode(h_bridge_pin_b, OUTPUT);
    digitalWrite(h_bridge_pin_a, LOW);
    digitalWrite(h_bridge_pin_b, LOW);
    // assign ledc channel

    this->__h_bridge_pin_a = h_bridge_pin_a;
    this->__h_bridge_pin_b = h_bridge_pin_b;

}

void ActuatorDcMotor::SpinOnce(){
    // real speed control, position check, auto stop....
}

float ActuatorDcMotor::GetCurrentPosition_InCncUnit(){
    return this->__sensor->getAngle();
}

void ActuatorDcMotor::SetCurrentPositionAs(float position_in_cnc_unit){

}

void ActuatorDcMotor::SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit){

}

float ActuatorDcMotor::GetDistanceToTarget_InCncUnit(){

}

void ActuatorDcMotor::SetSpeed(float speed_per_second){

}

void ActuatorDcMotor::Stop(){

}



void ActuatorDcMotor::StartToMove(){

}
