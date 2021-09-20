#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyLibs/MyFunctions.hpp"
#include "hardware.hpp"
#include "cablebot_corner.hpp"
#include "Robot/Gcode.h"

CableBotCorner<DCMotor,COMMU_T> robot = CableBotCorner<DCMotor,COMMU_T>(AXIS_NAME);

void setup(){
    setup_hardware();
    // link and couple the components;
    robot.LinkCommuDevice(&commu);
    robot.LinkAcuator(&motor);
    robot.LinkHomeTriger(&homeTriger);

    robot._actuator->linkDriver(&hBridge);
    robot._actuator->linkSensor(&encoder);

    robot._actuator->controller = MotionControlType::angle;
    robot._actuator->P_angle.P = 1;

    robot.Init_scaler(1.234) ;
    // Serial.println("Setup() is done");
    // Serial.print("aaaaaa");
    commu.OutputMessage("Setup() is done...\n");
}

void loop(){
    commu.SpinOnce();
    robot.SpinOnce();

    // Notificate my status.
    // ble.UpdateCurrentPos(100);

    if (robot.IsBusy()){
        return;
    }
    if(commu.HasNewChatting()){
        // ble got new gcode
    commu.OutputMessage("222222222222");
        std::string command(commu.ReadChatting());
    commu.OutputMessage("44444");
    commu.OutputMessage(command);
        Gcode gCode = Gcode(command);
        // Gcode gCode = Gcode(commu.ReadChatting());   //Risk for not releasing memory ?
    commu.OutputMessage("55555");
        robot.RunGcode(&gCode);
    commu.OutputMessage("66666");
    }
}

#endif