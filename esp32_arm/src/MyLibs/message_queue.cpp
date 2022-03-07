#include "message_queue.h"
#include "HardwareSerial.h"


bool MessageQueue::AppendMessage(const char* message, int length){

}

bool MessageQueue::BufferIsEmpty(){
    if (head==tail) return true;
    return false;
}

int MessageQueue::GetFreeBuffersCount(){
    int count = this->head - this->tail;
    if (count >= 0)
        return MESSAGE_COUNT_IN_QUEUE - count;
    else
        return 0-count;
}

void MessageQueue::SayHello(){
    Serial.println("\n\n GcodeQueue::SayHello()");
    Serial.print("\n    Hello, I am instance of  GcodeQueue, normally I am local gcode queue... ");
    Serial.print("\n    MAX_BYTES_PER_MESSAGE = ");
    Serial.print(MAX_BYTES_PER_MESSAGE);
    Serial.print("\n    MESSAGE_COUNT_IN_QUEUE = ");
    Serial.print(MESSAGE_COUNT_IN_QUEUE);
    Serial.print("\n         this->head= ");
    Serial.println(this->head);
    Serial.print("\n         this->tail= ");
    Serial.println(this->tail);
    // Serial.print("\n     &this->_mybot->State= ");
    // Serial.println(this->_myBot->);    
}