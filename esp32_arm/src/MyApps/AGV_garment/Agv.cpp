#include "Agv.h"


AGV::AGV(){
}

void AGV::Init(PIDController* wheel_pid, L298N* left_wheel, L298N* right_wheel){
    this->wheel_pid =  wheel_pid;
    this->leftWheel = left_wheel;
    this->rightWheel = right_wheel;
    
}
void AGV::Move(AGV_DIRECTION direction, int speed_in_percent){
    this->common_speed = 2.55f * speed_in_percent;


}

void AGV::Stop(){
    this->common_speed = 0;
    this->leftWheel->Stop();
    this->rightWheel->Stop();
}
int AGV::ReadRailSensor(){
    return 0;
}


void AGV::SpinOnce(){
    // Read sensor, then adjust speed of left wheel and right wheel.
    int error = ReadRailSensor();
    left_wheel_speed = common_speed + error;
    right_wheel_speed = common_speed - error;
}