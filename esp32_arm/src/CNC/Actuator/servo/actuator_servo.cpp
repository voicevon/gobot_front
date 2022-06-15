#include "actuator_servo.h"



void ActuatorServo::SetPosition(float position){
    // This function is called from CNC, Who send a rad-angle of position unit. 
    this->__current_position_in_rad = position;
    this->__position_offset_in_rad = position - DEG_TO_RAD * this->__servo->read();
}


float ActuatorServo::GetCurrentPosition(){
    return this->__current_position_in_rad;
}

void ActuatorServo::MoveTo(bool is_absolute_position, float position){
    if (!is_absolute_position){
        this->__current_position_in_rad += position;
        Serial.print("[debug] ActuatorServo::MoveTo() position= ");
        Serial.print(position);
        Serial.print("   ");
        Serial.println(this->__current_position_in_rad + position);

    }
    float physic_angle = RAD_TO_DEG * (this->__current_position_in_rad - this->__position_offset_in_rad);
    this->__servo->write(physic_angle);
}
