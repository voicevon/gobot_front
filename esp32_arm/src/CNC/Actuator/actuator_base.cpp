#include "actuator_base.h"

void ActuatorBase::LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint){
    this->_range_constraint = range_constraint;
    this->_is_range_constraint = true;
}


float ActuatorBase::GetNeededSeconds(){
    return this->GetDistanceToTarget_InCncUnit() / this->GetSpeed();
    
}

void ActuatorBase::RenewSpeed(float moving_time){
    float speed = this->GetDistanceToTarget_InCncUnit() / moving_time;
    this->SetSpeed(speed);
}
