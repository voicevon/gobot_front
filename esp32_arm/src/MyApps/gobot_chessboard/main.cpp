#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD


#include "hardware.hpp"
#include "Robot/Commu/CommuRobotBLE.h"
#include "gobot_chessboard.h"
#include "MyLibs/MyFunctions.hpp" 

// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboard* robot; 
RobotAction action;
CommuRobotBle ble= CommuRobotBle();

// void output_message(std::string message){
//     ble.WriteNotification(message.c_str()); 
//     SerialPrintString(message);
// }

void setup(){
    setup_hardware();
    ble.Init();
    Serial.println("BLE is ok....");    
    robot = &GobotChessboard::getInstance();
    //couple the components
    // robot->axis_alpha.LinkAcuator(&stepper_alpha);
    // robot->axis_beta.LinkAcuator(&stepper_beta);
    // robot->axis_alpha.LinkHomeTriger(&homeTriger_alpha);
    // robot->axis_beta.LinkHomeTriger(&homeTriger_beta);
    robot->LinkActuatorController(&stepControl);
    // robot->OnOutputMessage_set_callback(output_message);

    // robot->Setup(&action, 9);

    robot->HomeAllAxises();
    // Serial.print("\nGobot-Chessboard Homing is done......");
    Serial.print("\nGobot-Chessboard setup is done..........");

}


void loop(){
    ble.SpinOnce();
    robot->SpinOnce();

    // Notificate my status.
    // ble.UpdateCurrentPos(100);

    if (robot->IsBusy())
        return;
    if(ble.HasNewChatting()){
        // ble got new gcode
        Gcode gCode = Gcode(ble.ReadChatting());   //Risk for not releasing memory ?
        robot->RunGcode(&gCode);
    }
}



#endif