
#include "twh4_ship_out_app.h"
#include <HardwareSerial.h>


Twh4_ShipOut_App::Twh4_ShipOut_App(int row_id){
    this->__robot_row_id = row_id;
    Serial.print("\n[Info] Twh4_ShipOut_App::Twh4_ShipOut_App() is constructed");
}

void Twh4_ShipOut_App::onGot_MqttMessage(const char* command){
    String str_command = String(command);
    // Logger::Info("Twh4_ShipOut_App::ExecuteMqttCommand()");
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
    if (my_gcode.g == 1){
        if (my_gcode.get_value('R') == this->__robot_row_id){
            this->_gcode_queue->AppendGcodeCommand(command);
        }
    }

}




