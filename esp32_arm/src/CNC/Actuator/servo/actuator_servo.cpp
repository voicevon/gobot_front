#include "actuator_servo.h"



void ActuatorServo::SetPosition(float position){
    this->__current_position = position;
    this->__position_offset = position - this->__servo->read();
}


float ActuatorServo::GetCurrentPosition(){
    return this->__current_position;
}

void ActuatorServo::MoveTo(bool is_absolute_position, float position){
    if (!is_absolute_position){
        this->__current_position += position;
        Serial.print("[debug] ActuatorServo::MoveTo() position= ");
        Serial.print(position);
        Serial.print("   ");
        Serial.println(this->__current_position + position);

    }
    float physic_angle = this->__current_position - this->__position_offset;
    this->__servo->write(physic_angle);
}
