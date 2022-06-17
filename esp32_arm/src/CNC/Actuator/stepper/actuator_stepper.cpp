#include "actuator_stepper.h"


// void ActuatorStepper::LinkStepper(Stepper* stepper, bool inverse_stepper_dir, int32_t steps_per_cnc_unit){
void ActuatorStepper::LinkStepper(Stepper* stepper, ActuatorMechanicStepper* mechanic){
    this->__stepper = stepper;
    this->__steps_per_cnc_unit = mechanic->GetStepsPerCncUnit();
    // this->steps_per_cnc_unit = steps_per_cnc_unit;
    // this->inverse_direction = inverse_stepper_dir;
}


// void ActuatorStepper::LinkStepper(Stepper* stepper){this->__stepper=stepper;};
float ActuatorStepper::GetCurrentPosition_InCncUnit(){
    return 1.0f * this->__stepper->getPosition() / this->__steps_per_cnc_unit;    
}

float ActuatorStepper::GetDistanceToTarget_InCncUnit(){
    int32_t distance_in_step = this->__stepper->getDistanceToTarget();
    return 1.0f * distance_in_step / this->__steps_per_cnc_unit;
}

void ActuatorStepper::SetPosition(float position_in_cnc_unit){
    //TODO:  direction is here.
    int32_t position_in_step = position_in_cnc_unit * this->__steps_per_cnc_unit;
    this->__stepper->setPosition(position_in_step);
}

void ActuatorStepper::MoveTo(bool is_absolute_position, float position_in_cnc_unit){
    if (is_absolute_position){
        this->_target_cnc_position = position_in_cnc_unit;
    } else {
        this->_target_cnc_position += position_in_cnc_unit;
    }
    int32_t posititon_in_step = position_in_cnc_unit * this->__steps_per_cnc_unit;
    this->__stepper->setTargetAbs(posititon_in_step);
}