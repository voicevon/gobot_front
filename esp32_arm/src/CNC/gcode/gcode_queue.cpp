#include "gcode_queue.h"
#include "HardwareSerial.h"

GcodeQueue::GcodeQueue(){
}

// TODO: BLOCKING mode or UNBLOCKING mode
// return true:   buffer is full
// return false:  buffer is not full
bool GcodeQueue::AppendGcodeCommand(String command){
    return this->AppendMessage(command);
}

bool GcodeQueue::AppendGcodeCommand(const char* payload, int length){
    return this->AppendMessage(payload, length);
}



