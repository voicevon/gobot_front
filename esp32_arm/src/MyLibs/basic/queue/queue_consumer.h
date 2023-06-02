#pragma once
#include "queue_base.h"
#include "MyLibs/utility/logger.h"

class CommonQueueConsumer{
    public:
        void LinkQueue_AsConsumer(QueueBase* the_queue) {this->_queue=the_queue;};

    protected:
        QueueBase* _queue;
};