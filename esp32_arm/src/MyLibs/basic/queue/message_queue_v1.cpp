// #include "message_queue.h"
// #include "MyLibs/basic/logger.h"

// bool MessageQueue_v1::AppendMessage(String strPayload){
//     const char* payload = strPayload.c_str();
//     int length = strPayload.length();
//     // Serial.print("[Info] MessageQueue_v1::AppendMessage(String strPayload)   ");
//     // Serial.println(strPayload);
//     return this->AppendMessage(payload, length);
// }

// // return true: buffer is full , before or after appending.
// // return false:  buffer is not full, after appending.
// bool MessageQueue_v1::AppendMessage(const char* payload, int length){
//     // Logger::Debug("MessageQueue_v1::AppendMessage()");
//     // Logger::Print("payload", payload);
//     // Logger::Print("length",length);

//     int next_head = this->__get_pointer_next_index(this->_head);
//     if(next_head == this->_tail){
//         Serial.print(FORE_YELLOW);
//         Serial.print("\n  [Warn] MessageQueue_v1::AppendMessage() ");
//         Serial.print("\n   Buffer is full");
//         Serial.println(COLOR_RESET);
//         return true;
//     }
//     _all_messages[_head].length = length;
//     char* target = &(_all_messages[_head].payload[0]);
//     //Copy byte to byte.  There is a 0x00 ender , so lenth should be +1.
//     // Logger::Print("Copying message byte by byte......", ".");
//     for(int i=0;i<=length;i++){
//         // Serial.print(*payload);
//         *target = * payload;
//         target++;
//         payload++;
//     }
//     // Serial.println("\n\n");
//     _head = next_head;
//     next_head = __get_pointer_next_index(_head);
//     if (next_head == this->_tail)
//         // buffer is full, after copying.
//         return true;
//     // buffer is NOT full. 
//     return false;  
//     // // This doens't work, Don't know how Arduino String is orgnized with bytes.
//     // // for(int i=0; i< command.length() +1; i++){  // The extra one byte is 0x00, the string ender.
//     // // // for(int i=0; i< length +1; i++){  // The extra one byte is 0x00, the string ender.
//     // //     Serial.print(*pSourceByte);
//     // //     *pTargetByte = *pSourceByte;
//     // //     pSourceByte++;
//     // //     pTargetByte++;
//     // // } 
// }

// MessageQueue_v1::SingleMessage* MessageQueue_v1::GetHeadMessage(){
//     int previous_head = __get_pointer_previous_index(_head);
//     SingleMessage* head_message = &(_all_messages[previous_head]);
//     return  head_message;
// }

// MessageQueue_v1::SingleMessage* MessageQueue_v1::GetWithdrawTailElement(bool take_out_from_queue){
//     SingleMessage* tail_message = NULL;
//     if (_head != _tail){
//         tail_message = & _all_messages[_tail];
//         if(take_out_from_queue){
//             _tail = __get_pointer_next_index(_tail);
//         }
//     }
//     return tail_message;
// }

// bool MessageQueue_v1::BufferIsEmpty(){
//     if (_head == _tail) 
//         return true;
//     return false;
// }

// bool MessageQueue_v1::BufferIsFull(){
//     int next_head = __get_pointer_next_index(_head);
//     if (next_head == _tail){
//         Logger::Debug("MessageQueue_v1::BufferIsFull()");
//         Logger::Print("MessageQueue_v1::BufferIsFull()   point", 98);
//         return true;
//     }
//     // Logger::Print(" MessageQueue_v1::BufferIsFull()   point", 99);
//     return false;
// }

// int MessageQueue_v1::GetFreeBuffersCount(){
//     int count = this->_head - this->_tail;
//     if (count >= 0)
//         return MESSAGE_COUNT_IN_QUEUE - count;
//     else
//         return 0-count;
// }

// int MessageQueue_v1::__get_pointer_next_index(int current_index){
//     // Logger::Debug("MessageQueue_v1::__get_pointer_next_index");
//     int next_index = current_index;
//     next_index++;
//     if (next_index == MESSAGE_COUNT_IN_QUEUE){
//         // out of range.
//         next_index = 0;
//     }
//     // Logger::Print("MESSAGE_COUNT_IN_QUEUE", MESSAGE_COUNT_IN_QUEUE);
//     // Logger::Print("current_index", current_index);
//     // Logger::Print("next_index", next_index);
//     return next_index;
// }

// int MessageQueue_v1::__get_pointer_previous_index(int current_index){
//     int next_index = current_index;
//     next_index--;
//     if (next_index <0 )   
//         // out of range.
//         next_index = MESSAGE_COUNT_IN_QUEUE - 1;
//     return next_index;
// }

// void MessageQueue_v1::SayHello(const char* title){
//     Logger::Info(title);
//     Logger::Print("Hello, I am instance of  GcodeQueue, normally I am local gcode queue... ", "...");
//     Logger::Print("MAX_BYTES_PER_MESSAGE ", MAX_BYTES_PER_MESSAGE);
//     Logger::Print("MESSAGE_COUNT_IN_QUEUE ", MESSAGE_COUNT_IN_QUEUE);
//     Logger::Print("this->head ", this->_head);
//     Logger::Print("this->tail ", this->_tail);
//     Logger::Print("tail message payload = ", this->_all_messages[this->_tail].payload);
// }