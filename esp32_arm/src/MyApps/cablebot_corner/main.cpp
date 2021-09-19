#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyLibs/MyFunctions.hpp"
#include "hardware.hpp"
#include "cablebot_corner.hpp"
// #include "Robot/Commu/CommuBleGattServer.h"
#include "Robot/Gcode.h"
#include "Robot/Commu/CommuSingleAxisRobotBLE.h"
#include "Robot/Actuator/DcMotor.h"

// Top level component
CommuSingleAxisRobotBLE ble = CommuSingleAxisRobotBLE();
CableBotCorner<DCMotor> robot = CableBotCorner<DCMotor>(AXIS_NAME);
// Sub level component
DCMotor motor = DCMotor();

// void output_message(std::string message){
//     ble.WriteNotification(message.c_str()); 
//     SerialPrintString(message);
// }

void setup(){
    setup_hardware();
    Serial.println(" ------------------------- " );
    ble.Init();
    Serial.println(" 1111111111111111 " );

    // link and couple the components;
    robot.LinkCommuDevice(&ble);
    robot.LinkAcuator(&motor);
    robot.LinkHomeTriger(&homeTriger);
    Serial.println(" 222222222 " );

    robot._actuator->linkDriver(&hBridge);
    robot._actuator->linkSensor(&encoder);
    Serial.println(" 333333333 " );

    robot._actuator->controller = MotionControlType::angle;
    robot._actuator->P_angle.P = 1;
    Serial.println(" 44444444444 " );

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