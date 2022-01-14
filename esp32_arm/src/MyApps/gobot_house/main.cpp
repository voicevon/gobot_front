#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

// #include "hardware.hpp"
#include "Robot/Commu/CommuBleGattServer.h"
#include "gobot_house.h"
#include "MyLibs/MyFunctions.hpp" 
#include "Robot/command_queue.h"

// static char LOG_TAG[]= "BLE-HOUSE";
GobotHouse* mybot; 
RobotAction action;
CommandQueue myCommandQueue = CommandQueue();

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your bot,  Gobot-House. ");
    mybot = &GobotHouse::getInstance();
    mybot->Init_Linkage();
    // mybot->Setup(&action, 9);
	myCommandQueue.LinkRobot(mybot);
    String strG28 = "G28";
	bool result = myCommandQueue.AppendGcodeCommand(strG28);
    // Serial.println("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
	myCommandQueue.SpinOnce();
    Serial.print("\nHouse setup is done..........");
}

void loop(){
	String strG4 = "G4S";
	String strG1 = "G1X";
	// WebCommu_SpinOnce();
	mybot->SpinOnce();
	myCommandQueue.SpinOnce();
	if ((mybot->State == RobotBase::IDLE) && (myCommandQueue.BufferIsEmpty())){

		// // Go back to home position, keep a 0.1mm position
		// int speed = 540;
		// if (speed < 100) speed=100;
		// if (speed > 400000) speed=400000;  
		// String sg3 = strG1 + 123;
		// sg3.concat("F");
		// sg3.concat(speed);
		// bool result = myCommandQueue.AppendGcodeCommand(sg3);

		// // Push to the end position, might stop here.
		// int distance = 12;
		// String sg = strG1;
		// sg.concat(53);
		// result = myCommandQueue.AppendGcodeCommand(sg);

		// // sleep a while 
		// String sg2 = strG4 + 5;
		// result = myCommandQueue.AppendGcodeCommand(sg2);

	}

}


#endif