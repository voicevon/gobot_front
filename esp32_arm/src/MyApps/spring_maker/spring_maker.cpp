
#include "spring_maker.h"

SpringMaker::SpringMaker(){
	this->_gcode_queue = new GcodeQueue();
    Serial.print("\n[Info] SpringMaker::SpringMaker() is done..........");
}

void SpringMaker::ExecuteMqttCommand(const char* command){
    String str_command = String(command);
    Serial.print("[Info] SpringMaker::ExecuteMqttCommand() is entering.... = ");
    Serial.println(str_command);

    if (str_command.equals("home")){
        String gcode = "G28AI";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equalsIgnoreCase("big")){
        Serial.println("                        Start to make big spring....  ");
        String gcode = "G1 A100";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals(" ")){

    }else{
        Serial.print("[Warn] SpringMaker::ExecuteMqttCommand()  = ");
        Serial.println(command);
    }
    //TODO:: Translate command to gcode, then send to gcode_queue.
	// this->_gcode_queue->AppendGcodeCommand(command);

}

void SpringMaker::SpinOnce(){
    if (!this->_gcode_queue->BufferIsFull()){
        this->CheckMqttCommand();
    }
    this->CheckMqttCommand();
}

void SpringMaker::ParkArms(bool do_homing){
	Serial.print("\n[Debug] SpringMaker::ParkArms() is entering");
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
	String strG1 = "G1 Z5421 W5";
	this->_gcode_queue->AppendGcodeCommand(strG1);
}

void SpringMaker::LoadBox(){
    this->_gcode_queue->AppendGcodeCommand("G1 Z100");  // Lift the box-track.
    this->_gcode_queue->AppendGcodeCommand("G1 W1");  // Rotate the box-track. will load the box.

}

void SpringMaker::UnloadBox(){

}

