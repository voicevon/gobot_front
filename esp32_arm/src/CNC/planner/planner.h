#pragma once

#include "queue_move_block.h"
#include "queue_line_segment.h"
#include "CNC/arm_solution/arm_solution_base.h"



class Planner{
    public:
        bool IsPlanable();
        // void AppendLineSegment(LineSegment* line);

        ArmSolutionBase* arm_solution;    // should be here?  Only robot can manage arm_solution, right?
        LineSegment current_line;   //shoud be here?
        void ConvertLineSegment_AppendMoveBlocks(LineSegment* line);
    private:


};