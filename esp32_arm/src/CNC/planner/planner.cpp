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


void Planner::ConvertLineSegment_AppendMoveBlocks(LineSegment* line){
    line->Calculate_distance_time(&current_line.TargetPosition);
    IKPosition_abgdekl start_ik_position;
    IKPosition_abgdekl target_ik_position;

    Queue_MoveBlock::Instance().DeepCopyHead_ToPosition(&target_ik_position);
    arm_solution->IK(&line->TargetPosition, &target_ik_position);
    MoveBlock* mk = Queue_MoveBlock::Instance().GetRoom();
    mk->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_position.alpha;
    mk->MoveBlocks[AXIS_ALPHA].Recalulate_distance_speed_acceleration(start_ik_position.alpha, line->required_time);

    mk->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_position.beta;
    mk->MoveBlocks[AXIS_BETA].Recalulate_distance_speed_acceleration(start_ik_position.beta, line->required_time);

    mk->MoveBlocks[AXIS_GAMMA].TargetPosition = target_ik_position.gamma;
    mk->MoveBlocks[AXIS_GAMMA].Recalulate_distance_speed_acceleration(start_ik_position.gamma, line->required_time);

    mk->MoveBlocks[AXIS_DELTA].TargetPosition = target_ik_position.delta;
    mk->MoveBlocks[AXIS_DELTA].Recalulate_distance_speed_acceleration(start_ik_position.delta, line->required_time);
    
    Queue_MoveBlock::Instance().Deposit();

}






