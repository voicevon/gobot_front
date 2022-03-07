
#include "box_mover.h"

BoxMover::BoxMover(){
    BoxMoverHardware* objBoxMoverHardware = new BoxMoverHardware();
    objBoxMoverHardware->InitRobot();
	this->_gcode_queue = new GcodeQueue();
	this->_gcode_queue->LinkRobot(objBoxMoverHardware);
    Serial.print("\n[Info] BoxMover::BoxMover() is done..........");
}

void BoxMover::SpinOnce(){
    // this->verticalMover->SpinOnce();
    // this->angleMover->SpinOnce();
    this->_gcode_queue->SpinOnce();
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
		free_buffer_count = this->_gcode_queue->GetFreeBuffersCount();
		Serial.print(free_buffer_count);
		Serial.print(" ");
	}

	if (do_homing){
		String strG28 = "G28Z";
		this->_gcode_queue->AppendGcodeCommand(strG28);
		strG28 = "G28W";
		this->_gcode_queue->AppendGcodeCommand(strG28);
	}
	this->_gcode_queue->SpinOnce();
	String strG1 = "G1 Z5421 W5";
	this->_gcode_queue->AppendGcodeCommand(strG1);
}

void BoxMover::LoadBox(){
    // Vertical down.  Angle down, Triger gate
    this->_gcode_queue->AppendGcodeCommand("G1 Z100");  // Lift the box-track.
    this->_gcode_queue->AppendGcodeCommand("G1 W1");  // Rotate the box-track. will load the box.
    // this->_gcode_queue->AppendGcodeCommand("M123");   //???

}

void BoxMover::UnloadBox(){

}

