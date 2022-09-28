#pragma once

#include "move_block_queue.h"
#include "CNC/gcode/line_segment.h"




class Planner{
    public:
        bool IsPlanable();
        void AppendLineSegment(LineSegment* line);


        Queue_MoveBlock* __moveblock_queue;

    private:



};