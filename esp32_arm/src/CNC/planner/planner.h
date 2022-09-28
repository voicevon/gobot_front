#pragma once

#include "move_block_queue.h"
#include "CNC/gcode/line_segment.h"
#include "CNC/arm_solution/arm_solution_base.h"



class Planner{
    public:
        bool IsPlanable();
        void AppendLineSegment(LineSegment* line);


        ArmSolutionBase* __arm_solution;

    private:



};