
// #include "spring_maker.h
#include "vsc_robot.h"
#include <HardwareSerial.h>

// #include <Arduino.h>

VscRobot::VscRobot(){
    for (int i=0; i<LAYER_COUNT; i++){
        __position_diction[i] = 0;
    }

    //  0 ~ TWO_PI
    __position_diction[0] = 0.112;
    __position_diction[1] = 0.21;
    __position_diction[2] = 0.88;
    __position_diction[3] = 1.22;
    __position_diction[4] = 3.44;
    __position_diction[5] = 5.67;
    __position_diction[6] = 5.99;
    __position_diction[7] = 6.88;
    __position_diction[8] = 7.11;

    Serial.print("\n[Info] VscRobot::VscRobot() is constructed");
}

void VscRobot::ShowLayer(int layer_index){
    String g1 = "G1A";
    g1.concat(this->__actuator_slope *  this->__position_diction[layer_index]);
    this->_gcode_queue->AppendGcodeCommand(g1);
}

void VscRobot::AsyncExecuteMqttCommand(const char* command){
    String str_command = String(command);
    Logger::Info("VscRobot::ExecuteMqttCommand() is entering.");
    Logger::Print("Command", str_command);
    
    if (str_command.substring(0,9) == "ShowLayer"){
        int layer = atoi(str_command.substring(9).c_str());
        this->ShowLayer(layer);
    }else{
        this->_gcode_queue->AppendGcodeCommand(command);
    }
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


