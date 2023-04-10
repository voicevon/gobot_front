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
    int fbc = gs_Queue_MoveBlock::Instance().GetFreeBuffersCount();
    if (fbc > 3) return true;
    return false;
}


void Planner::ConvertLineSegment_AppendMoveBlocks(LineSegment* new_line){
    new_line->Calculate_distance_time(arm_solution->GetCurrentPosition_Fk());
    IKPosition_abgdekl start_ik;
    // Logger::Print("Planner::ConvertLineSegment_AppendMoveBlocks()  point", 21);
    gs_Queue_MoveBlock::Instance().GetHead_MoveBlock()->DeepCopyToIkPosition(&start_ik);
    // Logger::Print("Planner::ConvertLineSegment_AppendMoveBlocks()  point", 22);

    IKPosition_abgdekl target_ik;
    gs_Queue_MoveBlock::Instance().DeepCopyHead_ToPosition(&target_ik);
    arm_solution->FK_to_IK(&new_line->TargetPosition, &target_ik);
    MoveBlock* mb = gs_Queue_MoveBlock::Instance().GetRoom();
    // Logger::Print("Planner::ConvertLineSegment_AppendMoveBlocks()  point", 24);
    for(int i=0; i<CncActuator_List::Instance().GetItemsCount(); i++){
        // Logger::Print("Planner::ConvertLineSegment_AppendMoveBlocks()  point", 31);
        mb->MoveBlocks[i].TargetPosition = target_ik.Positions[i];
        CncActuatorBase* actuator = CncActuator_List::Instance().GetActuator(i);
        // Logger::Print("Planner::ConvertLineSegment_AppendMoveBlocks()  point", 32);
        actuator->Recalulate_distance_speed_acceleration(&mb->MoveBlocks[i], start_ik.Positions[i], new_line->Required_time);
        // Logger::Print("Planner::ConvertLineSegment_AppendMoveBlocks()  point", 33);
    }
    
    gs_Queue_MoveBlock::Instance().Deposit();

}






