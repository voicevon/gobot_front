#include "actuator_stepper.h"

// void ActuatorStepper::LinkStepper(Stepper* stepper){this->__stepper=stepper;};
float ActuatorStepper::GetCurrentPosition_InCncUnit(){
    return this->__stepper->getPosition();    
}

float ActuatorStepper::GetDistanceToTarget(){
    int32_t distance_in_step = this->__stepper->getDistanceToTarget();
}

void ActuatorStepper::SetPosition(float position){
    this->__stepper->setPosition(position);
}

void ActuatorStepper::MoveTo(bool is_absolute_position, float position_in_cnc_unit){
    this->_target_abs_position = position_in_cnc_unit;
}