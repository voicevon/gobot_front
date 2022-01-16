
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


void BoxMover::ResetToLoad(){

}
void BoxMover::LoadBox(){
    // Vertical down.  Angle down, Triger gate
    this->__commandQueue->AppendGcodeCommand("G1Z100");
    this->__commandQueue->AppendGcodeCommand("G1A123");
    this->__commandQueue->AppendGcodeCommand("M123");

}
void BoxMover::UnloadBox(){

}

