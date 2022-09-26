#pragma once

#include "line_segment.h"
#include "MyLibs/common_queue/common_queue.h"

class LineSegmentQueue: public CommonQueue {
    public:
        LineSegment* FetchTail_LineSegment(){return (LineSegment*)this->FetchTailObject(); };
        LineSegment* GetHead_LineSegment() {return (LineSegment*)this->GetHeadObject(); };

    private:


};