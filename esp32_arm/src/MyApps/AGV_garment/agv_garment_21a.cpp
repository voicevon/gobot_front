
#include "agv_garment_21a.h"


// void AgvGarment_21a::LinkWheels(ActuatorBase* left_wheel, ActuatorBase* right_wheel){
//     this->leftWheel = left_wheel;
//     this->rightWheel = right_wheel;
// }

void AgvGarment_21a::LinkPid(PIDController* speed_pid){
    this->speed_pid =  speed_pid;

}


void AgvGarment_21a::MoveForward(int track_error) {
    // int error = this->trackSensor->ReadError_ToRight();
    if (track_error == 888) {
        // this->Stop();
        // return;
    }

    if (track_error == 999) {
        track_error = this->_LastError;
    }else{
        this->_LastError = track_error;
    }
    float p = 3.4;
    this->leftWheel.driver->MoveAtSpeed(this->_TargetSpeed + p * track_error, FORWARD);
    this->rightWheel.driver->MoveAtSpeed(this->_TargetSpeed - p * track_error, FORWARD);
}

void AgvGarment_21a::Stop(){
    this->_TargetSpeed = 0;
    this->leftWheel.driver->Stop();
    this->rightWheel.driver->Stop();
}


