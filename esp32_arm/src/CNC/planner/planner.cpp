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
        MoveBlock* mb = this->__moveblock_queue->GetHeadMoveblock();
        Logger::Print("mb->axis  old", mb->axis);
        mb->axis = line->axis;
        mb->IsAbsTargetPosition = line->IsAbsTargetPosition;
        mb->TargetPosition = line->TargetPosition;
        mb->Speed = line->Speed;
        mb->Acceleration = line->Acceleration;

        Logger::Print("mb->axis  new", mb->axis);
        Logger::Print("Going to forward queue head", 0 );
        this->__moveblock_queue->ForwardHead();
        Logger::Print("Going to forward queue head", 1 );

    }
}






