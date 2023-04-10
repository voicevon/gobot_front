#include "MyLibs/common_queue/common_queue.h"

class GcodeQueue2: public CommonQueue{
    public:
        GcodeQueue2(){};
        
        // TODO: BLOCKING mode or UNBLOCKING mode
        // return true:   buffer is full
        // return false:  buffer is not full
        bool AppendGcodeCommand(String command){
            // return this->AppendMessage(command);
        };

        bool AppendGcodeCommand(const char* payload, int length){
            // return this->AppendMessage(payload, length);
        };

    private:
        
};