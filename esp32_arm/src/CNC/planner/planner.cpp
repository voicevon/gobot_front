#include "planner.h"


bool Planner::IsPlanable(){
    // Logger::Debug("Planner::IsPlanable()");
    // auto aa= this->__arm_solution;
    // Logger::Print("Planner::IsPlanable() point",1);
    // auto bb = aa->__queue_move_block;line
    // Logger::Print("Planner::IsPlanable() point",2);
    // auto cc = bb->GetFreeBuffersCount();
    // Logger::Print("Planner::IsPlanable() point",3);
    // int fbc = this->__arm_solution->__queue_move_block->GetFreeBuffersCount();
    int fbc = Queue_MoveBlock::Instance().GetFreeBuffersCount();
    if (fbc > 3) return true;
    return false;
}


//Doing:  Arm solution will help a lot of translation/transforming.
//  Convert FK line_segment to IK move_block.
// void Planner::AppendLineSegment(LineSegment* line){
//     //Translate LineSegment To MoveBlocks, and insert to move_block_queue.
//     Logger::Debug("Planner::AppendLineSegment()");
//     Logger::Print("line->axis", line->axis);
//     Queue_LineSegment::Instance().AppendObject(line);
//     // this->__arm_solution->__CutLineSegment_ToMoveBlocks_to_queue(line);
//     // this->__arm_solution->__ConvertSegment_ToMoveBlockQueue(line);
//     // for (int i=0; i<1; i++){
//     //     // TODO: recalculate acceleration, speed.
//     //     MoveBlock* mb = this->__arm_solution->__queue_move_block->GetHeadMoveblock();
//     //     for(int i=0; i<6;i++){
//     //         Logger::Print("mb->axis  old", mb->MoveBlocks[i].axis);
//     //         mb->MoveBlocks[i].axis = line->axis;
//     //         mb->MoveBlocks[i].IsAbsTargetPosition = line->IsAbsTargetPosition;
//     //         mb->MoveBlocks[i].TargetPosition = line->TargetPosition;
//     //         mb->MoveBlocks[i].Speed = line->Speed;
//     //         mb->MoveBlocks[i].Acceleration = line->Acceleration;

//     //         Logger::Print("mb->axis  new", mb->MoveBlocks[i].axis);
//     //     }
//     //     Logger::Print("Going to forward queue head", 0 );
//     //     this->__arm_solution->__queue_move_block->ForwardHead();
//     //     Logger::Print("Going to forward queue head", 1 );

//     // }
// }

void Planner::__ConvertLineSegment_AppendMoveBlocks(LineSegment* line){
    line->Calculate_distance_time(current_line.TargetPosition);
    IKPosition_abgdekl start_ik_position;
    IKPosition_abgdekl target_ik_position;

    Queue_MoveBlock::Instance().DeepCopyHead_ToPosition(&target_ik_position);
    __arm_solution->IK(line->TargetPosition, &target_ik_position);
    MoveBlock* mk = Queue_MoveBlock::Instance().GetRoom();
    // mk->MoveBlocks[AXIS_ALPHA].VectorDistance = start_ik_position.alpha - 
    mk->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_position.alpha;
    // mk->MoveBlocks[1].

    mk->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_position.beta;
    mk->MoveBlocks[AXIS_GAMMA].TargetPosition = target_ik_position.gamma;
    mk->MoveBlocks[AXIS_DELTA].TargetPosition = target_ik_position.delta;
    
    Queue_MoveBlock::Instance().Deposit();

}






