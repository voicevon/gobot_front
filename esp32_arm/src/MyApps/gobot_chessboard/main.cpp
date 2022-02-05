#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "gobot_chessboard.h"

#include "Robot/Commu/CommuBleGattServer.h"
#include "MyLibs/MyFunctions.hpp" 

// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboard* mybot; 
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
    mybot = &GobotChessboard::getInstance();
    mybot->Init();
    mybot->ParkArms(true);
    
    Serial.print("\nGobot-Chessboard setup is done..........");

}


void loop(){
    mybot->SpinOnce();
}



#endif