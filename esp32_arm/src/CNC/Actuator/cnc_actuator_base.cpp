#include "cnc_actuator_base.h"
#include <HardwareSerial.h>


void CncActuatorBase::LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint){
    this->_range_constraint = range_constraint;
    this->_is_range_constraint = true;
}


// float CncActuatorBase::GetNeededSeconds(){
//     //TODO: get time  (Distance, max_speed,  acceleration) 
//     // https://github.com/grbl/grbl/blob/master/grbl/planner.c
//     if (this->GetAbsDistanceToTarget_InCncUnit() != 0){
//         return this->GetAbsDistanceToTarget_InCncUnit() / this->GetSpeed();
//     }
//     return 0;
// }

// void CncActuatorBase::RenewSpeed(float moving_time){
//     float speed = this->GetAbsDistanceToTarget_InCncUnit() / moving_time;
//     bool debug = false;
//     if (debug){
//         Serial.print("[Debug] CncActuatorBase::RenewSpeed() new speed= ");
//         Serial.println(speed);
//     }
//     // this->SetSpeed(speed);
// }





void CncActuatorBase::ReInit_FormularOffset(float current_cnc_position){
    this->__raw_offset = current_cnc_position / this->__slope_from_raw_to_cnc - this->_current_position ;
}

float CncActuatorBase::ConvertPosition_ToCncUnit(float actuator_position){
    float cnc_position =  (actuator_position + this->__raw_offset) * this->__slope_from_raw_to_cnc;
    // return this->__ModTwoPi(cnc_position);  //??
    return cnc_position;  //??
}

float CncActuatorBase::GetPosition_FromCncUnit(float cnc_position){
    return cnc_position / this->__slope_from_raw_to_cnc - this->__raw_offset;
}


// float CncActuatorBase::__ModTwoPi(float origin_value){
//     int zoom = 100000;
//     float v = (int)(origin_value * zoom) % (int)(TWO_PI * zoom);
//     return v / zoom;
// }