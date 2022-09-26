#include "planner.h"


void Planner::SpinOnce(){
    Block* free_block = this->__block_queue.GetFreeBlock();
    if (free_block != nullptr){
        // Check lineSegment queue,
        LineSegment* line = this->__line_queue->GetLineSegment_ForConsumer();
        if(line != nullptr)
            this->__TranslateLineSegment_ToBlock(line);
    }
}

void Planner::__TranslateLineSegment_ToBlock(LineSegment* line){
    for (int i=0; i<1; i++){
        Block* bb = this->__block_queue.GetFreeBlock();
        bb->axis = line->axis;
        bb->IsAbsTargetPosition = line->IsAbsTargetPosition;
        bb->TargetPosition = line->TargetPosition;
        bb->Speed = line->Speed;
        bb->Acceleration = line->Acceleration;
    }
}



