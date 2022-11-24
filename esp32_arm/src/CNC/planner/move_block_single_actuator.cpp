#include "move_block_single_actuator.h"
#include "math.h"

// TODO: calculate speed and acceleration
void MoveBlock_SingleActuator::Recalulate_distance_speed_acceleration(float start_position, float the_time){
    this->VectorDistance = this->TargetPosition - start_position;
    this->Speed = fabs(VectorDistance) / the_time;
    this->Acceleration = 1;

}
