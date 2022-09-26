#pragma once

#include "block_queue.h"
#include "CNC/gcode/line_segment_queue.h"




class Planner{
    public:
        void AppendLineSegment();
        void SpinOnce();

    private:
        LineSegmentQueue* __line_queue;
        BlockQueue __block_queue;
        void __TranslateLineSegment_ToBlock(LineSegment* line);



};