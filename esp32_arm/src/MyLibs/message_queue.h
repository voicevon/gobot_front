#pragma once
#define MAX_BYTES_PER_MESSAGE 20
#define MESSAGE_COUNT_IN_QUEUE 22



class MessageQueue{
    public:
        struct SingleMessage{
            int id;
            char payload[MAX_BYTES_PER_MESSAGE];
            int length;
        };
        // void InitMessageQueue();
        bool AppendMessage(const char* message, int length);
        bool BufferIsEmpty();  //TODO: remove this function?
        bool BufferIsFull();
        int GetFreeBuffersCount();
        int GetHeadMessage(char* payload);
        // virtual void SpinOnce();
        // void HookConsumer(MessageConsumer* messageConsumer){};
        void SayHello();

    protected:
        // This is a buffer , can contain 20 bytes per command, and 22 commands in the queue.
        // char gCodeCommands[MAX_BYTES_PER_MESSAGE * MESSAGE_COUNT_IN_QUEUE];  // or [20][5]?
        SingleMessage _all_messages[MESSAGE_COUNT_IN_QUEUE];
        int _head = 0;
        int _tail = 0;
    private:

};