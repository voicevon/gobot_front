
#include "box_mover_app.h"

BoxMoverApp::BoxMoverApp(){
	// this->_gcode_queue = new GcodeQueue();
    Serial.print("\n[Info] BoxMoverApp::BoxMoverApp() is done..........");
}

// void BoxMoverApp::onGot_MqttMessae(const char* command){
// 	this->gcode_queue.AppendGcodeCommand(command);

// }

void BoxMoverApp::SpinOnce(){
    if (!this->gcode_queue.BufferIsFull()){
        this->CheckMqttCommand();
    }
    this->CheckMqttCommand();
    switch (this->State){
        case RESETTING:
            break;
        case LOADING:
            if(false){
                this->State = LOADED;
            }
            break;
        case LOADED:
            if (true){
            }
            break;
        case VERTICAL_UP:
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

void BoxMoverApp::ParkArms(bool do_homing){
	Serial.print("\n[Debug] BoxMoverApp::ParkArms() is entering");
	int free_buffer_count = 0;
	while (free_buffer_count < 3){
		this->SpinOnce();
		free_buffer_count = this->gcode_queue.GetFreeBuffersCount();
		Serial.print(free_buffer_count);
		Serial.print(" ");
	}

	if (do_homing){
		String strG28 = "G28Z";
		this->gcode_queue.AppendGcodeCommand(strG28);
		strG28 = "G28W";
		this->gcode_queue.AppendGcodeCommand(strG28);
	}
	// this->gcode_queue.SpinOnce();
	String strG1 = "G1 Z5421 W5";
	this->gcode_queue.AppendGcodeCommand(strG1);
}

void BoxMoverApp::LoadBox(){
    // Vertical down.  Angle down, Triger gate
    this->gcode_queue.AppendGcodeCommand("G1 Z100");  // Lift the box-track.
    this->gcode_queue.AppendGcodeCommand("G1 W1");  // Rotate the box-track. will load the box.
    // this->gcode_queue.AppendGcodeCommand("M123");   //???

}

void BoxMoverApp::UnloadBox(){

}

