
#include "spring_maker_app.h"

SpringMakerApp::SpringMakerApp(){
	this->_gcode_queue = new GcodeQueue();
    Serial.print("\n[Info] SpringMakerApp::SpringMakerApp() is done..........");
}

void SpringMakerApp::AsyncExecuteMqttCommand(const char* command){
    String str_command = String(command);
    Serial.print("[Info] SpringMakerApp::ExecuteMqttCommand() is entering.... = ");
    Serial.println(str_command);
    if (str_command.equals("home")){
        String gcode = "G28A";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals("reset")){
        String gcode = "G28A";
        this->_gcode_queue->AppendGcodeCommand(gcode);
        gcode = "G1 A-3";
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals("big")){
        Serial.println("                        Start to make big spring....  ");
        String gcode = "G1 A-75.36";       // 1.0mm   D ?? 12 turns.    
        this->_gcode_queue->AppendGcodeCommand(gcode);
        
    }else if(str_command.equals("mid")){
        String gcode = "G1 A-50.24";    // 8 turns
        this->_gcode_queue->AppendGcodeCommand(gcode);

    }else if(str_command.equals("small")){
        String gcode = "G1 A-300";     // 50 turns
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else if(str_command.equals("coil")){
        String gcode = "G1 A86.25 F5000";     // 700 turns without gearbox reducer
        this->_gcode_queue->AppendGcodeCommand(gcode);
    }else{
        Serial.print("[Warn] SpringMakerApp::ExecuteMqttCommand()  = ");
        Serial.println(command);
    }
}

// void SpringMakerApp::SpinOnce(){
//     if (!this->_gcode_queue->BufferIsFull()){
//         this->CheckMqttCommand();
//     }
//     this->CheckMqttCommand();
// }


