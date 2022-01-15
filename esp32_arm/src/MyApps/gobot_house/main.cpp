#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

#include "Robot/Commu/CommuBleGattServer.h"
#include "gobot_house.h"
#include "MyLibs/MyFunctions.hpp" 
// static char LOG_TAG[]= "BLE-HOUSE";
GobotHouse* mybot; 
RobotAction action;
CommandQueue myCommandQueue = CommandQueue();

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your lovely bot,  GobotHouse. ");
    mybot = &GobotHouse::getInstance();
    mybot->Setup(&action, 1);
	mybot->ParkArms(true);
}

void loop(){
	// WebCommu_SpinOnce();
	mybot->SpinOnce();

}


#endif