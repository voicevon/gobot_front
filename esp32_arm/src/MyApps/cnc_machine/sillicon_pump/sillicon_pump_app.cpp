
#include "sillicon_pump_app.h"
#include <HardwareSerial.h>


Silicon_Pump_App::Silicon_Pump_App(){
    Serial.print("\n[Info] Silicon_Pump_App::Silicon_Pump_App() is constructed");
}



void Silicon_Pump_App::onGot_MqttMessage(const char* command){
    this->_gcode_queue->AppendGcodeCommand("G92X0Y0");
    this->_gcode_queue->AppendGcodeCommand(command);   // Only accept "G1X123Y345"


        
}




