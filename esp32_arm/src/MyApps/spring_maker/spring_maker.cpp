
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
        String gcode = "G28A";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals("reset")){
        String gcode = "G1 A23";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals("big")){
        Serial.println("                        Start to make big spring....  ");
        String gcode = "G1 A100";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals("mid")){
        String gcode = "G1 A200";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals("small")){
        String gcode = "G1 A300";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else{
        Serial.print("[Warn] SpringMaker::ExecuteMqttCommand()  = ");
        Serial.println(command);
    }
}

void SpringMaker::SpinOnce(){
    if (!this->_gcode_queue->BufferIsFull()){
        this->CheckMqttCommand();
    }
    this->CheckMqttCommand();
}


