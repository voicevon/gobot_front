#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyLibs/MyFunctions.hpp"
#include "MyLibs/BleServerBase.h"
#include "hardware.hpp"
#include "cablebot_corner.hpp"
#include "Robot/Gcode.h"
#include "Robot/Axis/SingleAxisBLE.h"
#include "Robot/Actuator/DcMotor.h"

// Top level component
SingleAxisBLE ble = SingleAxisBLE();
CableBotCorner<DCMotor> robot = CableBotCorner<DCMotor>(AXIS_NAME);
// Sub level component
DCMotor motor = DCMotor();

void output_message(std::string message){
    ble.WriteNotification(message.c_str()); 
    SerialPrintString(message);
}

void setup(){
    ble.Init();
    setup_hardware();
    // link and couple the components;
    robot.OnOutputMessage_set_callback(output_message);

    robot.LinkAcuator(&motor);
    robot.LinkHomeTriger(&homeTriger);

    robot.actuator->linkDriver(&hBridge);
    robot.actuator->linkSensor(&encoder);

    robot.actuator->controller = MotionControlType::angle;
    robot.actuator->P_angle.P = 1;

    robot.Init_scaler(1.234) ;
}

void loop(){
    ble.SpinOnce();
    robot.SpinOnce();
    // Notificate my status.
    ble.UpdateCurrentPos(100);

    if (robot.IsBusy())
        return;
    if(ble.HasNewChatting()){
        // ble got new gcode
        Gcode gCode = Gcode(ble.ReadChatting());   //Risk for not releasing memory ?
        robot.RunGcode(&gCode);
    }
}

#endif