#include "planner.h"


bool Planner::IsPlanable(){
    int fbc = this->__moveblock_queue->GetFreeBuffersCount();
    if (fbc>3) return true;
    return false;
}

void Planner::AppendLineSegment(LineSegment* line){
    //Translate LineSegment To MoveBlocks, and insert to move_block_queue.
    Logger::Debug("Planner::AppendLineSegment()");
    Logger::Print("line->axis", line->axis);

    for (int i=0; i<1; i++){
        // TODO: recalculate acceleration, speed.
        MoveBlock* bq = this->__moveblock_queue->GetHeadMoveblock();
        Logger::Print("bq->axis", bq->axis);
        bq->axis = line->axis;
        bq->IsAbsTargetPosition = line->IsAbsTargetPosition;
        bq->TargetPosition = line->TargetPosition;
        bq->Speed = line->Speed;
        bq->Acceleration = line->Acceleration;
        this->__moveblock_queue->ForwardHead();
    }
}






