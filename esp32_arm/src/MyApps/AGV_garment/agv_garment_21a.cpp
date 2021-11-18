
#include "agv_garment_21a.h"


// void AgvGarment_21a::LinkWheels(ActuatorBase* left_wheel, ActuatorBase* right_wheel){
//     this->leftWheel = left_wheel;
//     this->rightWheel = right_wheel;
// }

void AgvGarment_21a::LinkPid(PIDController* speed_pid){
    this->speed_pid =  speed_pid;

}


void AgvGarment_21a::SpinOnce() {
    return;
    Serial.println(" BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB ");
    int error = this->trackSensor->ReadError_ToRight();
    Serial.println (error);
    this->leftWheel.driver->Start(this->_TargetSpeed + error, FORWARD);
    this->rightWheel.driver->Start(this->_TargetSpeed - error, FORWARD);
}

void AgvGarment_21a::Stop(){
    this->_TargetSpeed = 0;
    this->leftWheel.driver->Stop();
    this->rightWheel.driver->Stop();
}


