
#include "vsc_app.h"
#include <HardwareSerial.h>


VscApp::VscApp(){

}


void VscApp::AsyncExecuteMqttCommand(const char* command){
    String str_command = String(command);
    Logger::Info("VscApp::ExecuteMqttCommand() is entering.");
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
    if (str_command.substring(0,7) == "vsc_cmd"){
        // This is a robot command, not a gcode or mcode
        str_command = str_command.substring(7);
        int layer = my_gcode.get_value('L');
        int cell = my_gcode.get_value('C');
        Logger::Print("layer", layer);
        Logger::Print("cell", cell);
        // this->MoveTo(layer, cell);
        this->__robot->MoveTo(layer, cell);
    } else{
        // this is a normal gcode.
        this->_gcode_queue->AppendGcodeCommand(command);

    }

}




