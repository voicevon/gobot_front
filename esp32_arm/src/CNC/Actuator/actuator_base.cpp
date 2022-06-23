#include "actuator_base.h"
#include <HardwareSerial.h>


void ActuatorBase::LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint){
    this->_range_constraint = range_constraint;
    this->_is_range_constraint = true;
}


float ActuatorBase::GetNeededSeconds(){
    if (this->GetAbsDistanceToTarget_InCncUnit() != 0){
        return this->GetAbsDistanceToTarget_InCncUnit() / this->GetSpeed();
    }
    return 0;
}

void ActuatorBase::RenewSpeed(float moving_time){
    float speed = this->GetAbsDistanceToTarget_InCncUnit() / moving_time;
    bool debug = false;
    if (debug){
        Serial.print("[Debug] ActuatorBase::RenewSpeed() new speed= ");
        Serial.println(speed);
    }
    this->SetSpeed(speed);
}
