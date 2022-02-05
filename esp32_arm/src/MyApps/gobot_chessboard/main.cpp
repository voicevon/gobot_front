#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "gobot_chessboard_hw.h"

#include "Robot/Commu/CommuBleGattServer.h"
#include "MyLibs/MyFunctions.hpp" 

// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboardHardware* mybot; 
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
    mybot = &GobotChessboardHardware::getInstance();
    mybot->Init();
    Serial.print("\nGobot-Chessboard setup is done..........");

}


void loop(){
    // robot->SpinOnce();
    //for testing.
    Gcode gcode=Gcode("G28 X100");
    mybot->RunGcode(&gcode);

}



#endif