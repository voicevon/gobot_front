#include "message_queue.h"
#include "HardwareSerial.h"

bool MessageQueue::AppendMessage(String strPayload){
    const char* payload = strPayload.c_str();
    int length = strPayload.length();
    Serial.print("[Info] MessageQueue::AppendMessage(String strPayload)   ");
    Serial.println(strPayload);
    return this->AppendMessage(payload, length);
}

// return true: buffer is full , before or after appending.
// return false:  buffer is not full, after appending.
bool MessageQueue::AppendMessage(const char* payload, int length){
    int next_head = this->__get_pointer_next_index(this->_head);
    if(next_head == this->_tail){
        Serial.print("\n\n\n  [Error] MessageQueue::AppendMessage() ");
        Serial.print("\n   Buffer is full");
        return true;
    }
    this->_all_messages[this->_head].length = length;
    char* target = &(this->_all_messages[this->_head].payload[0]);
    //Copy byte to byte.  There is a 0x00 ender , so lenth should be +1.
    for(int i=0;i<=length;i++){
        *target = * payload;
        target++;
        payload++;
    }
    this->_head = next_head;
    next_head = this->__get_pointer_next_index(this->_head);
    if (next_head == this->_tail)
        // buffer is full, after copying.
        return true;
    // buffer is NOT full. 
    return false;  
    // // This doens't work, Don't know how Arduino String is orgnized with bytes.
    // // for(int i=0; i< command.length() +1; i++){  // The extra one byte is 0x00, the string ender.
    // // // for(int i=0; i< length +1; i++){  // The extra one byte is 0x00, the string ender.
    // //     Serial.print(*pSourceByte);
    // //     *pTargetByte = *pSourceByte;
    // //     pSourceByte++;
    // //     pTargetByte++;
    // // } 
}

MessageQueue::SingleMessage* MessageQueue::GetHeadMessage(){
    int previous_head = this->__get_pointer_previous_index(this->_head);
    SingleMessage* head_message = &(this->_all_messages[previous_head]);
    return  head_message;
}

MessageQueue::SingleMessage* MessageQueue::FetchTailMessage(){
    SingleMessage* tail_message = NULL;
    if (this->_head != this->_tail){
        tail_message = & this->_all_messages[this->_tail];
        this->_tail = this->__get_pointer_next_index(this->_tail);
    }
    return tail_message;
}

bool MessageQueue::BufferIsEmpty(){
    if (this->_head == this->_tail) 
        return true;
    return false;
}

bool MessageQueue::BufferIsFull(){
    int next_head = this->__get_pointer_next_index(this->_head);
    if (next_head == this->_tail)
        return true;
    return false;
}

int MessageQueue::GetFreeBuffersCount(){
    int count = this->_head - this->_tail;
    if (count >= 0)
        return MESSAGE_COUNT_IN_QUEUE - count;
    else
        return 0-count;
}

int MessageQueue::__get_pointer_next_index(int current_index){
    int next_index = current_index;
    next_index++;
    if (next_index == MESSAGE_COUNT_IN_QUEUE)   
        // out of range.
        next_index = 0;
    return next_index;
}

int MessageQueue::__get_pointer_previous_index(int current_index){
    int next_index = current_index;
    next_index--;
    if (next_index <0 )   
        // out of range.
        next_index = MESSAGE_COUNT_IN_QUEUE - 1;
    return next_index;
}

void MessageQueue::SayHello(String title){
    Serial.print("\n\n ========================================================");
    Serial.print("\n         GcodeQueue::SayHello(" + title + ")");
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
    Serial.print("\n tail message payload = ");
    Serial.print(this->_all_messages[this->_tail].payload);
    Serial.println("\n==================================== end ====");
}