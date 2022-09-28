#include "planner.h"


bool Planner::IsPlanable(){
    // Logger::Debug("Planner::IsPlanable()");
    // auto aa= this->__arm_solution;
    // Logger::Print("Planner::IsPlanable() point",1);
    // auto bb = aa->__queue_move_block;
    // Logger::Print("Planner::IsPlanable() point",2);
    // auto cc = bb->GetFreeBuffersCount();
    // Logger::Print("Planner::IsPlanable() point",3);
    int fbc = this->__arm_solution->__queue_move_block->GetFreeBuffersCount();
    if (fbc > 3) return true;
    return false;
}


//TODO:  Arm solution will help a lot of translation/transforming.
//  Convert FK line_segment to IK move_block.
void Planner::AppendLineSegment(LineSegment* line){
    //Translate LineSegment To MoveBlocks, and insert to move_block_queue.
    Logger::Debug("Planner::AppendLineSegment()");
    Logger::Print("line->axis", line->axis);

    for (int i=0; i<1; i++){
        // TODO: recalculate acceleration, speed.
        MoveBlock* mb = this->__arm_solution->__queue_move_block->GetHeadMoveblock();
        for(int i=0; i<6;i++){
            Logger::Print("mb->axis  old", mb->MoveBlocks[i].axis);
            mb->MoveBlocks[i].axis = line->axis;
            mb->MoveBlocks[i].IsAbsTargetPosition = line->IsAbsTargetPosition;
            mb->MoveBlocks[i].TargetPosition = line->TargetPosition;
            mb->MoveBlocks[i].Speed = line->Speed;
            mb->MoveBlocks[i].Acceleration = line->Acceleration;

            Logger::Print("mb->axis  new", mb->MoveBlocks[i].axis);
        }
        Logger::Print("Going to forward queue head", 0 );
        this->__arm_solution->__queue_move_block->ForwardHead();
        Logger::Print("Going to forward queue head", 1 );

    }
}






