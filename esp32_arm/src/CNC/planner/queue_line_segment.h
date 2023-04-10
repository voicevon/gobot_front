#pragma once

#include "line_segment.h"
#include "MyLibs/common_queue/common_queue.h"

#define LINE_SEGMENTS_QUEUE_SIZE 8


class gs_Queue_LineSegment: public CommonQueue{
    public:
        static gs_Queue_LineSegment& Instance(){
            static gs_Queue_LineSegment instance;
            return instance;
        };
        gs_Queue_LineSegment(){this->_Init("Line_Segments", LINE_SEGMENTS_QUEUE_SIZE, sizeof(LineSegment));};
        LineSegment* Withdraw() {return (LineSegment*)this->_Withdraw();};
        LineSegment* GetRoom() {return (LineSegment*)this->_GetRoom();};
        LineSegment* GetHeadLineSegment() {return (LineSegment*)this->_GetHeadObject();};
        void DeepCopyCurrentFkPositionTo(FKPosition_XYZRPW* position);
    private:
        

};