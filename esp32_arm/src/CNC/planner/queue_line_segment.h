#pragma once

#include "line_segment.h"
#include "MyLibs/common_queue/common_queue.h"

#define LINE_SEGMENT_QUEUE_SIZE 88


//TODO:  want a better name, like  MoveBlockQueueManager ?
class Queue_LineSegment: public CommonQueue{
    public:
        static Queue_LineSegment& Instance(){
            static Queue_LineSegment instance;
            return instance;
        };
        Queue_LineSegment(){this->_Init(LINE_SEGMENT_QUEUE_SIZE, sizeof(LineSegment));};
        LineSegment* FetchTailMoveBlock(){return (LineSegment*)this->FetchTailObject(); };
        LineSegment* GetHeadMoveblock() {return (LineSegment*)this->GetHeadObject(); };
    private:
        

};