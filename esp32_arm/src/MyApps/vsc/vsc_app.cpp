
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
    // Logger::Print("command",str_command);
    // Logger::Print("substring(0,7)", str_command.substring(0,7));
    // Logger::Print("substring(7)", str_command.substring(7));
    // Serial.print(str_command);
    // Serial.print("\t");
    // Serial.print(str_command.substring(0,7));
    // Serial.print("\t");
    // Serial.print(str_command.substring(7));
    // Serial.println();

    Gcode my_gcode = Gcode(command);
    if (str_command.substring(0,7) == "vsc_cmd"){
    // if (false){
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
    //TODO:  L3C8 == Show Layer 3, Cell 8.   target cell will be indicated by LED.
    // if (str_command.substring(0,1) == "L"){
    //     int C_position = str_command.indexOf('C');
    //     int layer = atoi(str_command.substring(9, C_position-1).c_str());
    //     int cell = atoi(str_command.substring(C_position+1).c_str());

    //     this->MoveTo(layer, cell);

    // }else{
    //     this->_gcode_queue->AppendGcodeCommand(command);
    // }
    // if (str_command.equals("stop")){
    //     this->__motor->Stop();
    // }

    // }else if(str_command.equals("reset")){
    //     String gcode = "G28A";
    //     this->_gcode_queue->AppendGcodeCommand(gcode);
    //     gcode = "G1 A-3";
    //     this->_gcode_queue->AppendGcodeCommand(gcode);
    // }else if(str_command.equals("big")){
    //     Serial.println("                        Start to make big spring....  ");
    //     String gcode = "G1 A-75.36";       // 1.0mm   D ?? 12 turns.    
    //     this->_gcode_queue->AppendGcodeCommand(gcode);
        
    // }else if(str_command.equals("mid")){
    //     String gcode = "G1 A-50.24";    // 8 turns
    //     this->_gcode_queue->AppendGcodeCommand(gcode);

    // }else if(str_command.equals("small")){
    //     String gcode = "G1 A-300";     // 50 turns
    //     this->_gcode_queue->AppendGcodeCommand(gcode);
    // }else if(str_command.equals("coil")){
    //     String gcode = "G1 A86.25 F5000";     // 700 turns without gearbox reducer
    //     this->_gcode_queue->AppendGcodeCommand(gcode);
    // }else{
    //     Serial.print("[Warn] VscApp::ExecuteMqttCommand()  = ");
    //     Serial.println(command);
    // }
}

void VscApp::SpinOnce(){
    if (!this->_gcode_queue->BufferIsFull()){
        this->CheckMqttCommand();
    }
    // this->CheckMqttCommand();
}


