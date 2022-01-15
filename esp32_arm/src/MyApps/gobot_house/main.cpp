#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

// #include "hardware.hpp"
#include "Robot/Commu/CommuBleGattServer.h"
#include "gobot_house.h"
// #include "gobot_house_hw.h"
#include "MyLibs/MyFunctions.hpp" 
// #include "Robot/command_queue.h"

// static char LOG_TAG[]= "BLE-HOUSE";
GobotHouse* mybot; 
RobotAction action;
CommandQueue myCommandQueue = CommandQueue();

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your lovely bot,  GobotHouse. ");
    mybot = &GobotHouse::getInstance();
	mybot->ParkArms(true);
    Serial.print("\n[Debug] GobotHouse::Setup() is done..........");
}

void loop(){
	// WebCommu_SpinOnce();
	mybot->SpinOnce();

}


#endif