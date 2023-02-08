
#include "twh_packer_app.h"
#include <HardwareSerial.h>


Twh_Packer_App::Twh_Packer_App(){
    // this->__robot_row_id = row_id;
    Serial.print("\n[Info] Twh_Packer_App::Twh_Packer_App() is constructed");
}

void Twh_Packer_App::onGot_MqttMessage(const char* command){
    String str_command = String(command);
    // Logger::Info("Twh_Packer_App::ExecuteMqttCommand()");
    bool debug = false;
    if (debug){
        Logger::Print("command",str_command);
        Logger::Print("substring(0,7)", str_command.substring(0,7));
        Logger::Print("substring(7)", str_command.substring(7));
        Serial.print(str_command);
        Serial.print("\t");
        Serial.print(str_command.substring(0,7));
        Serial.print("\t");
        Serial.print(str_command.substring(7));
        Serial.println();
    }
    Gcode my_gcode = Gcode(command);
    this->_gcode_queue->AppendGcodeCommand(command);
}




