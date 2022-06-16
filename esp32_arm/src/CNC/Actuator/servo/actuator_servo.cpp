#include "actuator_servo.h"



void ActuatorServo::SetPosition(float position){
    // This function is called from CNC, Who send a rad-angle of position's unit. 
    this->__current_position_in_rad = position;
    this->__position_offset_in_rad = position - DEG_TO_RAD * this->__servo->read();
}


float ActuatorServo::GetCurrentPosition(){
    return this->__current_position_in_rad;
}

void ActuatorServo::MoveTo(bool is_absolute_position, float position){
    if (is_absolute_position){
        this->__current_position_in_rad = position;
    }else{
        this->__current_position_in_rad += position;
    }

    bool debug = false;
    if(debug){
        Serial.print("[debug] ActuatorServo::MoveTo() position in degree = ");
        Serial.print(RAD_TO_DEG * position);
        Serial.print("   ");
        Serial.println(RAD_TO_DEG * (this->__current_position_in_rad + position));
    }
    float physic_angle = RAD_TO_DEG * (this->__current_position_in_rad - this->__position_offset_in_rad);
    this->__servo->write(physic_angle);
}


float ActuatorServo::__ToServoDegree(float from_cnc_rad){
    DEC_TO_RAD 
}

float ActuatorServo::__ToCncRad(float from_servo_degree){

}