#include "command_queue.h"
#include "HardwareSerial.h"

CommandQueue::CommandQueue(){
}

void CommandQueue::LinkRobot(RobotBase* mybot){
    this->_myBot = mybot;
    this->head = 0;
    this->tail = 0;
}

void CommandQueue::SpinOnce(){
    if (_myBot->State == RobotBase::IDLE){
        // Serial.println("Dispacher, Got robot idle.");
        if(this->head != this->tail){
            //Run next gcode
            Serial.print("\n\n         Start to run next gcode  ");
            char* p = &gCodeCommands[0];
            p += 20 * this->tail;
            std::string str = std::string(p);
            Serial.println(str.c_str());
            Gcode gcode = Gcode(str);
            this->_myBot->RunGcode(&gcode);
            this->tail++;
            if (this->tail == 5) this->tail = 0;
        }
    }
}

bool CommandQueue::BufferIsEmpty(){
    if (head==tail) return true;
    return false;
}

bool CommandQueue::AppendGcodeCommand(String command){
    int pre_head = this->head;
    pre_head++;
    if (pre_head == 5) pre_head =0;
    if (pre_head == this->tail){
        // Buffer is full
        return false;
    }
    // Serial.print("Adding gcode to command queue   ");
    // Serial.print(command.length());
    // Serial.print(">>> ");
    // Serial.println(command);
    unsigned char* pTargetByte = (unsigned char*) (&gCodeCommands[0]);
    pTargetByte += 20 * this->head;
    // This doens't work, Don't knwo how Arduino String is orgnized with bytes.
    // for(int i=0; i< command.length() +1; i++){  // The extra one byte is 0x00, the string ender.
    // // for(int i=0; i< length +1; i++){  // The extra one byte is 0x00, the string ender.
    //     Serial.print(*pSourceByte);
    //     *pTargetByte = *pSourceByte;
    //     pSourceByte++;
    //     pTargetByte++;
    // } 
    command.getBytes(pTargetByte, command.length() + 1);
    Serial.print("\nGcode added to command queue  >>> ");
    char* p = (char*)&gCodeCommands[0];
    p += 20 * this->head;
    std::string ss=std::string(p);
    Serial.println(ss.c_str());
    this->head = pre_head;
    return true;  
}


