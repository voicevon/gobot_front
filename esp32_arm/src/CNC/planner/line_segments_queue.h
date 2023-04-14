#pragma once

#include "line_segment.h"
#include "MyLibs/basic/queue/queue_base.h"

#define LINE_SEGMENTS_QUEUE_SIZE 8


class gs_LineSegment_Queue: public QueueBase<LineSegment>{
    public:
        static gs_LineSegment_Queue& Instance(){
            static gs_LineSegment_Queue instance;
            return instance;
        };

        void DeepCopyCurrentFkPositionTo(FKPosition_XYZRPW* position);

    private:
        LineSegment __all_line_segments[LINE_SEGMENTS_QUEUE_SIZE];
        gs_LineSegment_Queue(){this->_Init("Line_Segments", LINE_SEGMENTS_QUEUE_SIZE, __all_line_segments);};
        

};