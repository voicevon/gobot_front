#pragma once
#include "common_queue.h"
#include "MyLibs/basic/logger.h"

class CommonQueueConsumer{
    public:
        void LinkQueue_AsConsumer(CommonQueue* the_queue) {this->_queue=the_queue;};

    protected:
        CommonQueue* _queue;
};