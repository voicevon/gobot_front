#include "gcode_queue.h"

GcodeQueue::GcodeQueue(){
    this->_Init("GcodeQueue", GCODE_QUEUE_SIZE, this->__all_gcodes);
    // this->_all_elements = this->__all_gcodes;
}

int GcodeQueue::AppendGcodeCommand(String command){
    // return this->AppendMessage(command);
    Gcode* gcode =  this->_GetRoom();
    gcode->command = (char*) (command.c_str());
    this->Deposit();
    return 1;
};

int GcodeQueue::AppendGcodeCommand(const char* command){
    Logger::Debug("GcodeQueue::AppendGcodeCommand(const char* command)");
    this->PrintOut("caller is:  GcodeQueue::AppendGcodeCommand(const char* command)");

    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("GcodeQueue::AppendGcodeCommand() with auto-length,  queue is full");
        this->PrintOut("GcodeQueue::AppendGcodeCommand(const char* command)");
        return -1;
    }
    Logger::Print("GcodeQueue::AppendGcodeCommand(const char* command) point", 51);
    Gcode* gcode = this->_GetRoom();
    Logger::Print("GcodeQueue::AppendGcodeCommand(const char* command) point", 52);
    
    char* destination = gcode->command;
    Logger::Debug("GcodeQueue::AppendGcodeCommand");
    for (int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        *(destination + i) = command[i];
        if (command[i] == 0x00){
            Logger::Print("size of command string ", i+1);
            this->Deposit();
            return 1;
        }
    }
    return -2;
}


int GcodeQueue::AppendGcodeCommand(const char* payload, int length){
    // return this->AppendMessage(payload, length);
    if (this->GetFreeBuffersCount() == 0){
        Logger::Error("GcodeQueue::AppendGcodeCommand() queue is full");
        return -1;
    }
    if (length > REPRAP_GCODE_MAX_SIZE){
        Logger::Error("GcodeQueue::AppendGcodeCommand() payload is over-size");
        return -2;
    }
    Gcode* gcode = this->_GetRoom();
    char* destination = gcode->command;
    for (int i=0; i<length; i++){
        *(destination + i) = payload[i];
    }
    this->Deposit();
};