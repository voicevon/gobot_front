#include "cnc_actuator_base.h"
#include "von/utility/logger.h"



void CncActuatorBase::Recalulate_distance_speed_acceleration(MoveBlock_SingleActuator* move, float start_position, float target_time){
    move->VectorDistance = move->TargetPosition - start_position;
    move->Speed = fabs(move->VectorDistance) / target_time;
    // if (move->Speed > this->_limited_speed){
    //     move->Speed = this->_limited_speed;
    // }
    // move->Acceleration = this->_limited_accelleration;

}
