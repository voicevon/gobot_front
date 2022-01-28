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
    mybot->Setup(&action);
	mybot->ParkArms(true);
    mybot->ParkForCalibration(5);
}

bool done= false;
void loop(){
	// WebCommu_SpinOnce();
	mybot->SpinOnce();
    // done = mybot->MoveStone_FromRoomToHead(0);
    // if(done) Serial.print("Done to 0");
    // done = mybot->MoveStone_FromHeadToRoom(0);
    // if(done) Serial.print("Done to head");
    // mybot->MoveStone_FromRoomToHead(2);
    // mybot->MoveStone_FromRoomToHead(3);
    // mybot->MoveStone_FromRoomToHead(4);
    // mybot->MoveStone_FromRoomToHead(5);
    // mybot->MoveStone_FromRoomToHead(6);
    // mybot->MoveStone_FromRoomToHead(7);
}


#endif