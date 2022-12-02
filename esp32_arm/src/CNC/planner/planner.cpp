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


void Planner::ConvertLineSegment_AppendMoveBlocks(LineSegment* new_line){
    LineSegment* current_line = Queue_LineSegment::Instance().GetHeadLineSegment();
    // Logger::Debug("Planner::ConvertLineSegment_AppendMoveBlocks()");
    // Logger::Print("new_line->TargetPosition.X", new_line->TargetPosition.X);
    // Logger::Print("new_line->TargetPosition.Y", new_line->TargetPosition.Y);

    new_line->Calculate_distance_time(&current_line->TargetPosition);
    IKPosition_abgdekl start_ik;
    Queue_MoveBlock::Instance().GetHead_MoveBlock()->DeepCopyToIkPosition(&start_ik);

    IKPosition_abgdekl target_ik;
    Queue_MoveBlock::Instance().DeepCopyHead_ToPosition(&target_ik);
    arm_solution->IK(&new_line->TargetPosition, &target_ik);
    MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
    for(int a=0; a<CNC_ACTUATORS_IDEAL_COUNT; a++){
        mb->MoveBlocks[a].TargetPosition = target_ik.Positions[a];
        mb->MoveBlocks[a].Recalulate_distance_speed_acceleration(start_ik.Positions[a], new_line->Required_time);
    }
    
    Queue_MoveBlock::Instance().Deposit();

}






