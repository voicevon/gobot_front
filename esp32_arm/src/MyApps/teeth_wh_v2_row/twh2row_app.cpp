
#include "twh2row_app.h"
#include <HardwareSerial.h>


Twh2Row_App::Twh2Row_App(int row_id){
    this->__robot_row_id = row_id;
    Serial.print("\n[Info] Twh2Row_App::Twh2Row_App() is constructed");
}



void Twh2Row_App::AsyncExecuteMqttCommand(const char* command){
    String str_command = String(command);
    // Logger::Info("Twh2Row_App::ExecuteMqttCommand()");
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
    this->_gcode_queue->AppendGcodeCommand(command);
}




