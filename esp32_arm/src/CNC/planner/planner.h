#pragma once

#include "queue_move_block.h"
#include "queue_line_segment.h"
// #include "CNC/gcode/line_segment.h"
#include "CNC/arm_solution/arm_solution_base.h"



class Planner{
    public:
        bool IsPlanable();
        void AppendLineSegment(LineSegment* line);

        ArmSolutionBase* __arm_solution;    // should be here?  Only robot can manage arm_solution, right?
        float current_speed;
        FkPosition_XYZAB current_fk_position;
        IkPosition_AlphaBeta current_ik_psotion;
    private:



};