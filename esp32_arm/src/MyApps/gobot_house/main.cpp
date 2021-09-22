#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

// #include "hardware.hpp"
#include "Robot/Commu/CommuBleGattServer.h"
#include "gobot_house.h"
#include "MyLibs/MyFunctions.hpp" 

// static char LOG_TAG[]= "BLE-HOUSE";
GobotHouse* robot; 
RobotAction action;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your bot,  Gobot-House. ");
    robot = &GobotHouse::getInstance();
    robot->Init();
    robot->Setup(&action, 9);
    Serial.print("\nHouse setup is done..........");
}

void loop(){
    robot->SpinOnce();
    //for testing.
    Gcode gcode=Gcode("G28 X100");
    // robot->RunGcode(&gcode);
}


#endif