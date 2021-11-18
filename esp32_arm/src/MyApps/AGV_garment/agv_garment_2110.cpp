
#include "agv_garment_2110.h"


void AgvGarment_2110::LinkWheels(ActuatorBase* left_wheel, ActuatorBase* right_wheel){
    this->leftWheel = left_wheel;
    this->rightWheel = right_wheel;
}

void AgvGarment_2110::LinkPid(PIDController* speed_pid){
    this->speed_pid =  speed_pid;

}

// void Move(AGV_DIRECTION direction, int speed_in_percent) override;

void AgvGarment_2110::SpinOnce() {
    int error = this->lineSensor->ReadError_ToRight();
    // this->leftWheel->SetTargetSpeed(this->_TargetSpeed + error);
    // this->rightWheel->SetTargetSpeed(this->_TargetSpeed - error);
    this->leftWheel->driver->Start(this->_TargetSpeed + error, FORWARD);
    this->rightWheel->driver->Start(this->_TargetSpeed - error, FORWARD);
}
