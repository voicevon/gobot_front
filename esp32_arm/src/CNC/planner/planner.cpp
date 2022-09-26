#include "planner.h"


void Planner::SpinOnce(){
    int fbc = this->__block_queue.GetFreeBuffersCount();
    if (fbc >=3 ){
        // Check lineSegment queue,
        if (! this->__line_queue->BufferIsEmpty()){
            LineSegment* line = this->__line_queue->FetchTail_LineSegment();
            this->__TranslateLineSegment_ToBlock(line);
        }
    }
}

void Planner::__TranslateLineSegment_ToBlock(LineSegment* line){
    for (int i=0; i<1; i++){
        MoveBlock* bq = (MoveBlock*) this->__block_queue.FetchTailObject();
        bq->axis = line->axis;
        bq->IsAbsTargetPosition = line->IsAbsTargetPosition;
        bq->TargetPosition = line->TargetPosition;
        bq->Speed = line->Speed;
        bq->Acceleration = line->Acceleration;
    }
}



