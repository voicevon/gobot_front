#pragma once

#include "queue_move_block.h"
#include "queue_line_segment.h"
#include "CNC/arm_solution/arm_solution_base.h"



class Planner{

    public:

        ArmSolutionBase* arm_solution; 
        // LineSegment current_line;   //shoud be here?
        bool IsPlanable();
        void ConvertLineSegment_AppendMoveBlocks(LineSegment* line);
    
    private:


};