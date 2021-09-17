#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "hardware.hpp"
#include "MyLibs/BleServerBase.h"
#include "Robot/SingleAxisBase.hpp"
#include "Robot/SingleAxisBLE.h"
#include "Robot/Actuator/DcMotor.h"

// BleHelper bleHelper = BleHelper();

SingleAxisBLE ble = SingleAxisBLE();
DCMotor motor = DCMotor();
SingleAxisBase<DCMotor> axis = SingleAxisBase<DCMotor>();


void setup(){
    setup_hardware();

    ble.Init();
    // ble.AppendGattChar(0,12);

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
    uint16_t* xx;
    ble.GetTargetPositionAbs();
    uint16_t pos_X = *(xx+3);
    axis.SetTargetAbs(pos_X);
    axis.MoveAsync();
    ble.UpdateCurrentPos(100);
}

#endif