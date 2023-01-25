
#include "twh2row_app.h"
#include <HardwareSerial.h>


Twh2Row_App::Twh2Row_App(int row_id){
    this->__robot_row_id = row_id;
    Serial.print("\n[Info] Twh2Row_App::Twh2Row_App() is constructed");
}



// void Twh2Row_App::onGot_MqttMessage(const char* command){
//     String str_message_type = String(command);
//     // Logger::Info("Twh2Row_App::ExecuteMqttCommand()");
//     bool debug = false;
//     if (debug){
//         Logger::Print("command",str_message_type);
//         Logger::Print("substring(0,7)", str_command.substring(0,7));
//         Logger::Print("substring(7)", str_command.substring(7));
//         Serial.print(str_command);
//         Serial.print("\t");
//         Serial.print(str_command.substring(0,7));
//         Serial.print("\t");
//         Serial.print(str_command.substring(7));
//         Serial.println();
//     }

//     if (str_message_type.substring(0,3) == "cmd"){
//         //this is a app command

//     }
//     if (str_message_type.substring(0,3) == "gmc"){
//         // this is a robot gcode or mcode
//         this->_gcode_queue->AppendGcodeCommand(command);
//     }
// }




