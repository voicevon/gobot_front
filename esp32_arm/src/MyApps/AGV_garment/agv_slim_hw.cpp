
#include "agv_slim_hw.h"


// void AgvSlimHardware::LinkWheels(ActuatorBase* left_wheel, ActuatorBase* right_wheel){
//     this->leftWheel = left_wheel;
//     this->rightWheel = right_wheel;
// }

void AgvSlimHardware::LinkPid(PIDController* speed_pid){
    this->speed_pid =  speed_pid;

}
void AgvSlimHardware::Init(){
    // Setting PWM channels properties, Totally Esp32 has 16 channels
    const int freq = 30000;
    const int resolution = 8;   // so max pwm speed is 255
    ledcSetup(PWM_CHANNEL_0, freq, resolution); // configure LED PWM functionalitites
    ledcSetup(PWM_CHANNEL_1, freq, resolution); 


    // Init I2C bus
    Wire.begin();

    // Init AGV
    objLeftWheelBridge.Init(PWM_CHANNEL_0, PIN_LEFT_WHEEL_DC_MOTOR_ENABLE, PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
    objRightWheelBridge.Init(PWM_CHANNEL_1, PIN_RIGHT_WHEEL_DC_MOTOR_ENABLE, PIN_RIGHT_WHEEL_DC_MOTOR_A, PIN_RIGHT_WHEEL_DC_MOTOR_B);
    PIDController* speed_pid = new PIDController(1.0f, 1.0f, 0.0f ,80.0f, 100.0f);
    this->leftWheel.LinkDriver(&this->objLeftWheelBridge);
    this->rightWheel.LinkDriver(&this->objRightWheelBridge);
    //    this->agv_21a.LinkTrackSensor(&this->objTrackSensor_i2c);
    this->LinkPid(speed_pid);

}

void AgvSlimHardware::MoveForward(int track_error) {
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

void AgvSlimHardware::Stop(){
    this->_TargetSpeed = 0;
    this->leftWheel.driver->Stop();
    this->rightWheel.driver->Stop();
}


