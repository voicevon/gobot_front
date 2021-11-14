#include "Agv.h"


AGV::AGV(){
}

void AGV::Init(PIDController* wheel_pid){
    this->wheel_pid =  wheel_pid;
}
void AGV::Move(AGV_DIRECTION direction, int speed_in_percent){
    this->common_speed = 2.55f * speed_in_percent;


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