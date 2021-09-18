#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "hardware.hpp"
#include "MyLibs/BleServerBase.h"
// #include "Robot/Axis/SingleAxisBase.hpp"
// #include "Robot/Axis/SingleAxis.hpp"
#include "cablebot_corner.hpp"
#include "Robot/Axis/SingleAxisBLE.h"
#include "Robot/Actuator/DcMotor.h"
#include "Robot/Gcode.h"
// #include "Robot/SingleAxisRobot.hpp"
// Top level component
SingleAxisBLE ble = SingleAxisBLE();
CableBotCorner<DCMotor> robot = CableBotCorner<DCMotor>(AXIS_NAME);
// CableBotCorner<DCMotor> robot = CableBotCorner<DCMotor>(char(65));
// Sub level component
DCMotor motor = DCMotor();

void on_Axis_Fininshed_Moving(string message){
    ble.WriteNotification(message.c_str()); 
}

void setup(){
    ble.Init();
    setup_hardware();
    // link and couple the components;
    robot.LinkAcuator(&motor);
    robot.LinkHomeTriger(&homeTriger);

    robot.actuator->linkDriver(&hBridge);
    robot.actuator->linkSensor(&encoder);

    robot.actuator->controller = MotionControlType::angle;
    robot.actuator->P_angle.P = 1;

    robot.Init_scaler(1.234) ;
    robot.Home();
}

void loop(){
    ble.SpinOnce();
    robot.SpinOnce();
    // Notificate my status.
    ble.UpdateCurrentPos(100);

    if (robot.IsBusy())
        return;
        // axis is idle, not the first time.
    if(ble.HasNewChatting()){
        // ble got new gcode
        Gcode gCode = Gcode(ble.ReadChatting());   //Risk for not releasing memory ?
        robot.RunGcode(&gCode);
    }
}

#endif