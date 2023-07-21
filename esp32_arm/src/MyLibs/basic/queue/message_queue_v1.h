// #pragma once
// #define MAX_BYTES_PER_MESSAGE 20
// #define MESSAGE_COUNT_IN_QUEUE 16
// #include <WString.h>
// #include "von/utility/logger.h"
// #include "queue_base.h"


// class MessageBase{
//     public:

//     private:
// };


// // class MessageQueue: public QueueBase<MessageBase>{
// class MessageQueue_v1{
//     public:
//         struct SingleMessage{
//             int id;
//             char payload[MAX_BYTES_PER_MESSAGE];
//             int length;
//         };
        
//         // For Producer:    
//         bool AppendMessage(String payload);
//         bool AppendMessage(const char* payload, int length);
//         bool BufferIsFull();
//         int GetFreeBuffersCount();

//         // For Consumer:
//         bool BufferIsEmpty();  //TODO: remove this function?
//         SingleMessage* GetWithdrawTailElement(bool takeout_from_queue);

//         // For both Producer and Consumer:
//         MessageQueue(){};
//         SingleMessage* GetHeadMessage();
//         void SayHello(const char* title);

//     protected:
//         // This is a buffer , can contain 20 bytes per command, and 16 commands in the queue.
//         SingleMessage _all_messages[MESSAGE_COUNT_IN_QUEUE];
//         int _head = 0;
//         int _tail = 0;

//     private:
//         int __get_pointer_next_index(int current_index);
//         int __get_pointer_previous_index(int current_index);

// };