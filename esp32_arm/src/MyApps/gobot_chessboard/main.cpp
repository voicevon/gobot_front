#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "Robot/Commu/CommuBleGattServer.h"
#include "gobot_chessboard.h"
#include "MyLibs/MyFunctions.hpp" 

// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboard* robot; 
RobotAction action;
// CommuRobotBle ble= CommuRobotBle();

// void output_message(std::string message){
//     ble.WriteNotification(message.c_str()); 
//     SerialPrintString(message);
// }

void setup(){
    Serial.begin(115200);
    // setup_hardware();
    // ble.Init();
    // Serial.println("BLE is ok....");    
    robot = &GobotChessboard::getInstance();
    robot->Init_Linkage();
    Serial.print("\nGobot-Chessboard setup is done..........");

}


void loop(){
    // robot->SpinOnce();
    //for testing.
    Gcode gcode=Gcode("G28 X100");
    robot->RunGcode(&gcode);

}



#endif