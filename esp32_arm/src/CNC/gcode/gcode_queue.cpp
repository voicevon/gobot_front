#include "gcode_queue.h"

GcodeQueue::GcodeQueue(){
    this->_Init("GcodeQueue", GCODE_QUEUE_SIZE, this->__all_gcodes);
    GcodeText gcode("M999");

    Logger::Debug("GcodeQueue::GcodeQueue()");
    for(int i=0; i<GCODE_QUEUE_SIZE; i++){
        // this->__all_gcodes[0] = GcodeText;
        Logger::Print("index", i);
    }
    
    // this->_all_elements = this->__all_gcodes;
}

int GcodeQueue::AppendGcodeCommand(String command){
    // return this->AppendMessage(command);
    GcodeText* gcode =  this->GetRoom();
    // gcode->command = (char*) (command.c_str());
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
    GcodeText* gcode = this->GetRoom();
    Logger::Print("GcodeQueue::AppendGcodeCommand(const char* command) point", 52);
    // Logger::Print("command", gcode->command);
    char* destination = gcode->bytes;
    Logger::Debug("GcodeQueue::AppendGcodeCommand");
    for (int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        Logger::Print("command[i]" , command[i]);
        Logger::Print("destination[i]" , *(destination+i));
        // *(destination + i) = command[i];
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
    GcodeText* gcode = this->GetRoom();
    // char* destination = gcode[i];
    for (int i=0; i<length; i++){
        // *(destination + i) = payload[i];
    }
    this->Deposit();
};