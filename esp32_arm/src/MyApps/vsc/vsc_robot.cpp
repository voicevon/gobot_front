
// #include "spring_maker.h
#include "vsc_robot.h"
#include <HardwareSerial.h>

VscRobot::VscRobot(){
    Serial.print("\n[Info] VscRobot::VscRobot() is done..........");
}

void VscRobot::ShowLayer(int layer_index){
    float a= 123.45 * layer_index;
    String g1 = "G1A";
    g1.concat(a);
    this->_gcode_queue->AppendGcodeCommand(g1);
}

void VscRobot::AsyncExecuteMqttCommand(const char* command){
    String str_command = String(command);
    Serial.print("[Info] VscRobot::ExecuteMqttCommand() is entering.... = ");
    Serial.println(str_command);
    
    if (str_command.equals("stop")){
        this->__motor->Stop();
    }

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
    //     Serial.print("[Warn] VscRobot::ExecuteMqttCommand()  = ");
    //     Serial.println(command);
    // }
}

void VscRobot::SpinOnce(){
    // if (!this->_gcode_queue->BufferIsFull()){
    //     this->CheckMqttCommand();
    // }
    this->CheckMqttCommand();
}


