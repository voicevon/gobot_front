
// #include "loop_porter_app.h"
#include "sillicon_pump_app.h"
#include <HardwareSerial.h>


Sillicon_Pump_App::Sillicon_Pump_App(){
    // this->__robot_row_id = row_id;
    Serial.print("\n[Info] Sillicon_Pump_App::Sillicon_Pump_App() is constructed");
}



void Sillicon_Pump_App::onGot_MqttMessage(const char* command){
    this->_gcode_queue->AppendGcodeCommand("G92X0Y0");
    this->_gcode_queue->AppendGcodeCommand(command);   // Only accept "G1X123Y345"


        
}




