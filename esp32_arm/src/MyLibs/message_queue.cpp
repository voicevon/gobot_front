#include "message_queue.h"
#include "HardwareSerial.h"

// void MessageQueue::InitMessageQueue(){
//     Serial.print("\n[Info] MessageQueue::InitMessageQueue() is entering");
//     this->head = 0;
//     this->tail = 0;
// }
bool MessageQueue::AppendMessage(const char* payload, int length){

}

int MessageQueue::GetHeadMessage(char* payload){
    // payload = & this->gCodeCommands + MESSAGE_COUNT_IN_QUEUE * this->head;
    return 9;
}

bool MessageQueue::BufferIsEmpty(){
    if (this->_head == this->_tail) return true;
    return false;
}

int MessageQueue::GetFreeBuffersCount(){
    int count = this->_head - this->_tail;
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
    Serial.println(this->_head);
    Serial.print("\n         this->tail= ");
    Serial.println(this->_tail);
    // Serial.print("\n     &this->_mybot->State= ");
    // Serial.println(this->_myBot->);    
    Serial.println("==================================== end ====");
}