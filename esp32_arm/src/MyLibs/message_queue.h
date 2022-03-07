#pragma once
#define MAX_BYTES_PER_MESSAGE 20
#define MESSAGE_COUNT_IN_QUEUE 22

class MessageQueue{
    public:
        bool AppendMessage(const char* message, int length);
        bool BufferIsEmpty();
        int GetFreeBuffersCount();
        virtual void SpinOnce();
        void SayHello();

    protected:
        // This is a buffer , can contain 20 bytes per command, and 22 commands in the queue.
        char gCodeCommands[MAX_BYTES_PER_MESSAGE * MESSAGE_COUNT_IN_QUEUE];  // or [20][5]?
        int head;
        int tail;
    private:

};