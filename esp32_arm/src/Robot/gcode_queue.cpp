#include "gcode_queue.h"
#include "HardwareSerial.h"

GcodeQueue::GcodeQueue(){
}

void GcodeQueue::LinkRobot(RobotBase* mybot){
    Serial.print("\n[Info] CommmandQueue::LinkRobot() is entering");
    this->_myBot = mybot;
    this->head = 0;
    this->tail = 0;
    Serial.print("\n[Info] CommmandQueue::LinkRobot() is done.");
}

void GcodeQueue::SpinOnce(){
    if (_myBot->State == RobotState::IDLE){
        // Serial.println("Dispacher, Got robot idle.");
        if(this->head != this->tail){
            //Run next gcode
            Serial.print("\n[Info] GcodeQueue::SpinOnce() Start to run next gcode  ");
            char* p = &gCodeCommands[0];
            p += MAX_BYTES_PER_MESSAGE * this->tail;
            std::string str = std::string(p);
            Serial.println(str.c_str());
            Gcode gcode = Gcode(str);
            this->_myBot->RunGcode(&gcode);
            this->tail++;
            if (this->tail == MESSAGE_COUNT_IN_QUEUE) this->tail = 0;
        }
    }
    this->_myBot->SpinOnce();
}



bool GcodeQueue::AppendGcodeCommand(String command){
    // Serial.print("\n GcodeQueue::AppendGcodeCommand()  is entering..... ");
    int pre_head = this->head;
    pre_head++;
    if (pre_head == MESSAGE_COUNT_IN_QUEUE)   
        pre_head =0;
        
    while (pre_head == this->tail){
        // Buffer is full
        // Serial.println("\n\n\n\n [Warning] GcodeQueue::AppendGcodeCommand()   Buffer is full \n\n\n");
        this->SpinOnce();
    }
    // Serial.print("Adding gcode to command queue   ");
    // Serial.print(command.length());
    // Serial.print(">>> ");
    // Serial.println(command);
    unsigned char* pTargetByte = (unsigned char*) (&gCodeCommands[0]);
    pTargetByte += MAX_BYTES_PER_MESSAGE * this->head;
    // This doens't work, Don't know how Arduino String is orgnized with bytes.
    // for(int i=0; i< command.length() +1; i++){  // The extra one byte is 0x00, the string ender.
    // // for(int i=0; i< length +1; i++){  // The extra one byte is 0x00, the string ender.
    //     Serial.print(*pSourceByte);
    //     *pTargetByte = *pSourceByte;
    //     pSourceByte++;
    //     pTargetByte++;
    // } 
    command.getBytes(pTargetByte, command.length() + 1);
    Serial.print("\n[Info] GcodeQueue::AppendGcodeCommand() Gcode added to command queue:  ");
    char* p = (char*)&gCodeCommands[0];
    p += MAX_BYTES_PER_MESSAGE * this->head;
    std::string ss=std::string(p);
    Serial.println(ss.c_str());
    this->head = pre_head;
    return true;  
}



