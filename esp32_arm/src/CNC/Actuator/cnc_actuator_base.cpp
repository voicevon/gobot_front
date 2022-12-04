#include "cnc_actuator_base.h"
#include "MyLibs/basic/logger.h"


void CncActuatorBase::LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint){
    this->_range_constraint = range_constraint;
    this->_is_range_constraint = true;
}

// void CncActuatorBase::Formular_SetRawOffset(float current_cnc_position){
//     this->__formular_raw_offset = current_cnc_position / this->__formular_slope_from_raw_to_cnc - this->_current_position ;
// }

// float CncActuatorBase::ConvertPosition_ToCncUnit(float actuator_position){
//     float cnc_position =  (actuator_position + this->__formular_raw_offset) * this->__formular_slope_from_raw_to_cnc;
//     // return this->__ModTwoPi(cnc_position);  //??
//     return cnc_position;  //??
// }

float CncActuatorBase::GetPosition_FromCncUnit(float cnc_position){
    // return cnc_position / this->__formular_slope_from_raw_to_cnc - this->__formular_raw_offset;
}

float CncActuatorBase::GetSpeed_FromCncUnit(float cnc_speed){
    // return cnc_speed / this->__formular_slope_from_raw_to_cnc;
}

void CncActuatorBase::Recalulate_distance_speed_acceleration(MoveBlock_SingleActuator* move, float start_position, float target_time){
    move->VectorDistance = move->TargetPosition - start_position;
    move->Speed = fabs(move->VectorDistance) / target_time;
    if (move->Speed > this->_limited_speed){
        move->Speed = this->_limited_speed;
    }
    move->Acceleration = this->_limited_accelleration;

}
