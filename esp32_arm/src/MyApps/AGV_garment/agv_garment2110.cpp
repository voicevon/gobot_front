
#include "agv_garment2110.h"


void AgvGarment::Init(PIDController* speed_pid, ActuatorBase* left_wheel, ActuatorBase* right_wheel){
    this->speed_pid =  speed_pid;
    this->leftWheel = left_wheel;
    this->rightWheel = right_wheel;
}

void AgvGarment::_SpinOnce_Enter() {
    this->_track_pos_error = this->ReadTrackSensor();
    this->leftWheel->SetTargetSpeed(this->common_speed + this->_track_pos_error) ;
    this->rightWheel->SetTargetSpeed(this->common_speed - this->_track_pos_error) ;
}
