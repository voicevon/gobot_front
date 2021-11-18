#include "agv_base.h"


AgvBase::AgvBase(){
}

// void AgvBase::Init(PIDController* wheel_pid, L298N* left_wheel, L298N* right_wheel){

void AgvBase::Move(AGV_DIRECTION direction, int speed_in_percent){
    this->common_speed = 2.55f * speed_in_percent;
}

void AgvBase::Stop(){
    this->common_speed = 0;
    // this->leftWheelDriver->Stop();
    // this->rightWheelDriver->Stop();
}
int AgvBase::ReadTrackSensor(){
    return 0;
}


void AgvBase::SpinOnce(){
    // Read sensor, then adjust speed of left wheel and right wheel.
    this->_track_pos_error = ReadTrackSensor();

    // left_wheel_speed = common_speed + error;
    // right_wheel_speed = common_speed - error;
}