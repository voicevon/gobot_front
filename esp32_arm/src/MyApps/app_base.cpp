#include "app_base.h"

void AppBase::SpinOnce(){
    if (!this->_gcode_queue->BufferIsFull()){
        this->CheckMqttCommand();
    }
}

void AppBase::onGot_MqttMessage(const char* message){
    String str_message = String(message);
    // Logger::Info("Twh2Row_App::ExecuteMqttCommand()");
    bool debug = false;
    if (debug){
        Logger::Print("command",str_message);
        Logger::Print("substring(0,7)", str_message.substring(0,7));
        Logger::Print("substring(7)", str_message.substring(7));
        Serial.print(str_message);
        Serial.print("\t");
        Serial.print(str_message.substring(0,7));
        Serial.print("\t");
        Serial.print(str_message.substring(7));
        Serial.println();
    }
    String payload_id ;
    String payload_value = str_message.substring(4);
    if (str_message.substring(0,3) == "cmd"){
        //this is a app command

    }
    if (str_message.substring(0,3) == "gmc"){
        // this is a robot gcode or mcode

        Logger::Print("substring(7)", str_message.substring(7));

        this->_gcode_queue->AppendGcodeCommand(payload_value);
    }
}

