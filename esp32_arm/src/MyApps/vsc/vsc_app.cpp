
#include "vsc_app.h"
#include <HardwareSerial.h>


VscApp::VscApp(){
    for (int i=0; i<LAYER_COUNT; i++){
        __position_in_pitch[i] = 0;
    }

    //  0 ~ 156
    this->__position_in_pitch[0] = 0;
    this->__position_in_pitch[1] = 13;
    this->__position_in_pitch[2] = 26;
    this->__position_in_pitch[3] = 39;
    this->__position_in_pitch[4] = 52;
    this->__position_in_pitch[5] = 65;
    this->__position_in_pitch[6] = 78;
    this->__position_in_pitch[7] = 91;
    this->__position_in_pitch[8] = 104;
    this->__position_in_pitch[9] = 117;
    this->__position_in_pitch[10] = 130;
    this->__position_in_pitch[11] = 143;
    this->__position_in_pitch[12] = 156;
    this->__position_in_pitch[13] = 169;
    this->__position_in_pitch[14] = 182;

    Serial.print("\n[Info] VscApp::VscApp() is constructed");
}

void VscApp::MoveTo(int layer_index, int cell_index){
    // String m123 = "M123C";
    // m123.concat(cell_index);
    // this->_gcode_queue->AppendGcodeCommand(m123);

    String g1 = "G1A";
    // TODO:  over single circle.
    float gear_angle = TWO_PI *  this->__position_in_pitch[layer_index] / 184;
    g1.concat(gear_angle);
    g1.concat("F0.1");
    this->_gcode_queue->AppendGcodeCommand(g1);
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
        this->MoveTo(layer, cell);
    } else{
        // this is a normal gcode.
        this->_gcode_queue->AppendGcodeCommand(command);

    }

}




