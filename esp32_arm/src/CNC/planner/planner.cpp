#include "planner.h"


bool Planner::IsPlanable(){
    int fbc = this->__block_queue.GetFreeBuffersCount();
    if (fbc>3) return true;
    return false;
}

void Planner::AppendLineSegment(LineSegment* line){
    //Translate LineSegment ToBlocks
    for (int i=0; i<1; i++){
        MoveBlock* bq = (MoveBlock*) this->__block_queue.FetchTailObject();
        bq->axis = line->axis;
        bq->IsAbsTargetPosition = line->IsAbsTargetPosition;
        bq->TargetPosition = line->TargetPosition;
        bq->Speed = line->Speed;
        bq->Acceleration = line->Acceleration;
    }
}






