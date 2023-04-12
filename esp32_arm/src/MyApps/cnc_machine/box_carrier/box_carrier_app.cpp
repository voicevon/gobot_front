
#include "box_carrier_app.h"

BoxCarrierApp::BoxCarrierApp(){
    Serial.print("\n[Info] BoxCarrierApp::BoxCarrierApp() is done..........");
}

// void BoxCarrierApp::onGot_MqttMessage(const char* command){
// 	this->gcode_queue.AppendGcodeCommand(command);
// }

void BoxCarrierApp::SpinOnce(){
    if (!this->gcode_queue.BufferIsFull()){
        // this->CheckMqttCommand();
    }
    // this->CheckMqttCommand();
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

void BoxCarrierApp::ParkArms(bool do_homing){
	Serial.print("\n[Debug] BoxCarrierApp::ParkArms() is entering");
	int free_buffer_count = 0;
	while (free_buffer_count < 3){
		this->SpinOnce();
		free_buffer_count = this->gcode_queue.GetFreeBuffersCount();
		Serial.print(free_buffer_count);
		Serial.print(" ");
	}

	if (do_homing){
		// const char strG28 = "G28Z";
		this->gcode_queue.AppendGcodeCommand("G28Z");
		// const char = "G28W";
		this->gcode_queue.AppendGcodeCommand("G28W");
	}
	// this->gcode_queue.SpinOnce();
	// const char strG1 = "G1 Z5421 W5";
	this->gcode_queue.AppendGcodeCommand("G1 Z5421 W5");
}

void BoxCarrierApp::LoadBox(){
    // Vertical down.  Angle down, Triger gate
    this->gcode_queue.AppendGcodeCommand("G1 Z100");  // Lift the box-track.
    this->gcode_queue.AppendGcodeCommand("G1 W1");  // Rotate the box-track. will load the box.
    // this->gcode_queue.AppendGcodeCommand("M123");   //???

}

void BoxCarrierApp::UnloadBox(){

}

