
#include "agv_garment_2110.h"


void AgvGarment_2110::Init(PIDController* speed_pid, ActuatorBase* left_wheel, ActuatorBase* right_wheel){
    this->speed_pid =  speed_pid;
    this->leftWheel = left_wheel;
    this->rightWheel = right_wheel;
}

void AgvGarment_2110::_SpinOnce_Enter() {
    int error = this->lineSensor->ReadError_ToRight();
    this->leftWheel->SetTargetSpeed(this->common_speed + error);
    this->rightWheel->SetTargetSpeed(this->common_speed - error);
}
