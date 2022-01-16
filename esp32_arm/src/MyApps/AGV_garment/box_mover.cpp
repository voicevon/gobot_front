
#include "box_mover.h"

BoxMover::BoxMover(){
    this->__robot_hardware = new BoxMoverHardware();
    // this->__robot_hardware = &GobotHouseHardware::getInstance();
    this->__robot_hardware->Init_Linkage();
	this->__commandQueue = new CommandQueue();
	this->__commandQueue->LinkRobot(this->__robot_hardware);
    Serial.print("\n[Debug] GobotHouse::Setup() is done..........");
}

void BoxMover::SpinOnce(){
    // this->verticalMover->SpinOnce();
    // this->angleMover->SpinOnce();
    switch (this->State){
        case RESETTING:
            // if (verticalMover->AT_NORTH && angleMover->AT_SOUTH){
            //     this->State = READY_TO_LOAD;
            // }
            break;
        case LOADING:
            if(false){
                this->State = LOADED;
            }
            break;
        case LOADED:
            if (true){
                // this->verticalMover->MoveToNorth();
                // this->State = VERTICAL_UP;
            }
            break;
        case VERTICAL_UP:
            // if (this->verticalMover->State == DualEndActuator::AT_NORTH){
            //     this->State = READY_TO_UNLOAD;
            // }
            break;
        case UNLOADING:
            if(false){
                this->State = UNLOADED;
            }
            break;
        default:
            break;
    }
}


void BoxMover::ParkArms(bool do_homing){
	Serial.print("\n[Debug] BoxMover::ParkArms() is entering");
	int free_buffer_count = 0;
	while (free_buffer_count < 3){
		this->SpinOnce();
		free_buffer_count = this->__commandQueue->GetFreeBuffersCount();
		Serial.print(free_buffer_count);
		Serial.print(" ");
	}

	if (do_homing){
		String strG28 = "G28Z";
		bool result = this->__commandQueue->AppendGcodeCommand(strG28);
		strG28 = "G28A";
		result = this->__commandQueue->AppendGcodeCommand(strG28);
	}
	this->__commandQueue->SpinOnce();
	String strG1 = "G1 Z5421 A1345";
	this->__commandQueue->AppendGcodeCommand(strG1);
}
void BoxMover::LoadBox(){
    // Vertical down.  Angle down, Triger gate
    this->__commandQueue->AppendGcodeCommand("G1 Z100");
    this->__commandQueue->AppendGcodeCommand("G1 A123");
    this->__commandQueue->AppendGcodeCommand("M123");

}
void BoxMover::UnloadBox(){

}

