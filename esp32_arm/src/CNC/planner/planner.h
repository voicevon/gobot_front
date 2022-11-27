#pragma once

#include "queue_move_block.h"
#include "queue_line_segment.h"
#include "CNC/arm_solution/arm_solution_base.h"



class Planner{

    public:

        ArmSolutionBase* arm_solution; 
        bool IsPlanable();
        void ConvertLineSegment_AppendMoveBlocks(LineSegment* line);
    
    private:


};