#include "actuator_stepper.h"


void ActuatorStepper::LinkStepper(Stepper* stepper, ActuatorMechanicStepper* mechanic){
    this->__stepper = stepper;
    this->__steps_per_cnc_unit = mechanic->GetStepsPerCncUnit();
}


float ActuatorStepper::GetCurrentPosition_InCncUnit(){
    //todo:  fit into cnc range
    return 1.0f * this->__stepper->getPosition() / this->__steps_per_cnc_unit;    
}

float ActuatorStepper::GetDistanceToTarget_InCncUnit(){
    int32_t distance_in_step = this->__stepper->getDistanceToTarget();
    return 1.0f * distance_in_step / this->__steps_per_cnc_unit;
}

float ActuatorStepper::__fit_into_joint_range(float cnc_position_in_rad){
    int range_min_in_degree = -315;
    int range_max_in_degree = 20;
    float new_position_in_cnc_unit = cnc_position_in_rad;
    if(cnc_position_in_rad > DEG_TO_RAD * range_max_in_degree){
        new_position_in_cnc_unit = cnc_position_in_rad - TWO_PI;
    }
    return new_position_in_cnc_unit;
}

void ActuatorStepper::SetPosition(float position_in_cnc_unit){
    //TODO:  direction is here.  The joint has range limitation.

    float new_position_in_cnc_unit = this->__fit_into_joint_range(position_in_cnc_unit);
    int32_t position_in_step = new_position_in_cnc_unit * this->__steps_per_cnc_unit;
    this->__stepper->setPosition(position_in_step);
    bool debug = true;
    if(debug){
        Serial.print("[Debug] ActuatorStepper::SetPosition() __steps_per_cnc_unit= ");
        Serial.print(this->__steps_per_cnc_unit);
        Serial.print("  cnc position in degree = ");
        Serial.print(RAD_TO_DEG * position_in_cnc_unit);
        Serial.print("  Stepper position= ");
        Serial.println(position_in_step);
    }
}

void ActuatorStepper::MoveTo(bool is_absolute_position, float position_in_cnc_unit){
    int32_t posititon_in_step = position_in_cnc_unit * this->__steps_per_cnc_unit;
    if (is_absolute_position){
        float new_position_in_cnc_unit = this->__fit_into_joint_range(position_in_cnc_unit);
        this->_target_cnc_position = new_position_in_cnc_unit;
        posititon_in_step = new_position_in_cnc_unit * this->__steps_per_cnc_unit;
        this->__stepper->setTargetAbs(posititon_in_step);
        bool debug = true;
        if (debug){
            Serial.print("[Debug] ActuatorStepper::MoveTo()   setPositionAbs(posititon_in_step= ");
            Serial.println (posititon_in_step);
        }
    } else {
        this->_target_cnc_position += position_in_cnc_unit;
        this->__stepper->setTargetRel(posititon_in_step);
    }
    bool debug = false;
    if(debug){
        Serial.print("[Debug] ActuatorStepper::MoveTo() steps_per_cnc_unit=");
        Serial.print(this->__steps_per_cnc_unit);
        Serial.print("  position_in_step= ");
        Serial.print(posititon_in_step);
        Serial.print("  _target_cnc_position degree= ");
        Serial.println(RAD_TO_DEG * this->_target_cnc_position);
    }
}

#define MAX_STEPS_PER_SECOND 4000
void ActuatorStepper::SetSpeed(float speed_per_second){
    float steps_per_second = speed_per_second * this->__steps_per_cnc_unit;
    if (steps_per_second > MAX_STEPS_PER_SECOND){
        steps_per_second = MAX_STEPS_PER_SECOND;
    }
    this->__stepper->setMaxSpeed(steps_per_second);
}

