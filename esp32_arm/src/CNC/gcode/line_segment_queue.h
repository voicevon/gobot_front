#pragma once

#include "line_segment.h"

class LineSegmentQueue{
    public:
        LineSegment* GetFreeLineSegment_ForProducer();
        LineSegment* GetLineSegment_ForConsumer();

    private:


};