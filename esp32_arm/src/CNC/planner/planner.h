#pragma once

#include "move_blocks_queue.h"
#include "line_segments_queue.h"
#include "CNC/arm_solution/arm_solution_base.h"



class Planner{

    public:

        ArmSolutionBase* arm_solution; 
        bool IsPlanable();
        void ConvertLineSegment_AppendMoveBlocks(LineSegment* line);
    
    private:


};