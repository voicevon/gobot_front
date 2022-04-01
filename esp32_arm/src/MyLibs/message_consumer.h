#pragma once 
#include "message_queue.h"

//  Known sub class
//       Any robot_hardware who run Gcode
//       Any Agv who run on map.  ??
class MessageConsumer{
    public:
        // virtual void FetchMessage(char* message, int length);
        void LinkLocalMessageQueue(MessageQueue* mq){this->_mq = mq;};

    protected:
        MessageQueue* _mq;

};