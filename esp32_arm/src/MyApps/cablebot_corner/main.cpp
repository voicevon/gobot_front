#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "hardware.hpp"
#include "MyLibs/BleServerBase.h"
// #include "Robot/Axis/SingleAxisBase.hpp"
#include "Robot/Axis/SingleAxis.hpp"
#include "Robot/Axis/SingleAxisBLE.h"
#include "Robot/Actuator/DcMotor.h"
#include "Robot/Gcode.h"
// Top level component
SingleAxisBLE ble = SingleAxisBLE();
SingleAxis<DCMotor> axis = SingleAxis<DCMotor>(AXIS_NAME);
// Gcode gCode = Gcode("G28 X");

// Sub level component
DCMotor motor = DCMotor();

void on_Axis_Fininshed_Moving(string message){
    ble.WriteNotification(message.c_str()); 
}

void setup(){
    setup_hardware();
    // link and couple the components;
    axis.LinkAcuator(&motor);
    axis.LinkHomeTriger(&homeTriger);

    motor.linkDriver(&hBridge);
    motor.linkSensor(&encoder);
    ble.Init();

    motor.controller = MotionControlType::angle;
    motor.P_angle.P = 1;

    axis.Init_scaler(1.234) ;
    axis.Home();
}

void loop(){
    ble.SpinOnce();
    axis.SpinOnce();
    // Notificate my status.
    ble.UpdateCurrentPos(100);

    if (axis.IsBusy())
        return;
        // axis is idle, not the first time.
    if(ble.HasNewChatting()){
        // ble got new gcode
        Gcode gCode = Gcode(ble.ReadChatting());   //Risk for not releasing memory ?
        axis.RunGcode(&gCode);
    }
}

#endif