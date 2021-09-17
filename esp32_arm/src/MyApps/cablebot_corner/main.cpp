#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "hardware.hpp"
#include "MyLibs/BleHelper.h"
#include "Robot/SingleAxisBase.hpp"

SingleAxisBase<DCMotor> axis = SingleAxisBase<DCMotor>();
BleHelper bleHelper = BleHelper();
DCMotor motor = DCMotor();


void setup(){
    setup_hardware();

    bleHelper.InitBle();
    motor.linkDriver(&hBridge);
    motor.linkSensor(&encoder);
    motor.controller = MotionControlType::angle;
    motor.P_angle.P = 1;

    axis.Init_scaler(1.234) ;
    axis.LinkAcuator(&motor);
    // axis.LinkSensor(&encoder);
    axis.Home();
}


void loop(){
    axis.SetTargetAbs(100);
    axis.MoveAsync();
}

#endif