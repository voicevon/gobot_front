#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyLibs/MyFunctions.hpp"
#include "hardware.hpp"
#include "cablebot_corner.hpp"
#include "Robot/Gcode.h"
#include "Robot/Actuator/DcMotor.h"

// Top level component
CableBotCorner<DCMotor> robot = CableBotCorner<DCMotor>(AXIS_NAME);
// Sub level component
DCMotor motor = DCMotor();

void setup(){
    setup_hardware();
    // link and couple the components;
    robot.LinkCommuDevice(&ble);
    robot.LinkAcuator(&motor);
    robot.LinkHomeTriger(&homeTriger);

    robot._actuator->linkDriver(&hBridge);
    robot._actuator->linkSensor(&encoder);

    robot._actuator->controller = MotionControlType::angle;
    robot._actuator->P_angle.P = 1;

    robot.Init_scaler(1.234) ;
    Serial.println("Setup() is done");
}

void loop(){
    ble.SpinOnce();
    robot.SpinOnce();
    // Notificate my status.
    // ble.UpdateCurrentPos(100);

    if (robot.IsBusy())
        return;
    if(ble.HasNewChatting()){
        // ble got new gcode
        Gcode gCode = Gcode(ble.ReadChatting());   //Risk for not releasing memory ?
        robot.RunGcode(&gCode);
    }
}

#endif