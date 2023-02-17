
// #include "loop_porter_app.h"
#include "sillicon_pump_app.h"
#include <HardwareSerial.h>


Sillicon_Pump_App::Sillicon_Pump_App(){
    // this->__robot_row_id = row_id;
    Serial.print("\n[Info] Sillicon_Pump_App::Sillicon_Pump_App() is constructed");
}



void Sillicon_Pump_App::onGot_MqttMessage(const char* command){
        this->_gcode_queue->AppendGcodeCommand(command);


        
    // String str_message_type = String(command);
    // // Logger::Info("Sillicon_Pump_App::ExecuteMqttCommand()");
    // bool debug = false;
    // if (debug){
    //     Logger::Print("command",str_message_type);
    //     Logger::Print("substring(0,7)", str_command.substring(0,7));
    //     Logger::Print("substring(7)", str_command.substring(7));
    //     Serial.print(str_command);
    //     Serial.print("\t");
    //     Serial.print(str_command.substring(0,7));
    //     Serial.print("\t");
    //     Serial.print(str_command.substring(7));
    //     Serial.println();
    // }

    // if (str_message_type.substring(0,3) == "cmd"){
    //     //this is a app command

    // }
    // if (str_message_type.substring(0,3) == "gmc"){
    //     // this is a robot gcode or mcode
    //     this->_gcode_queue->AppendGcodeCommand(command);
    // }
}




