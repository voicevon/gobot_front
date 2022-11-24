#include "queue_move_block.h"



void Queue_MoveBlock::DeepCopyHead_ToPosition(IKPosition_abgdekl* ik_position){
    MoveBlock* head_mb = this->GetHead_MoveBlock();
    ik_position->alpha = head_mb->MoveBlocks[AXIS_ALPHA].TargetPosition;
    ik_position->beta = head_mb->MoveBlocks[AXIS_BETA].TargetPosition;
    ik_position->gamma =head_mb->MoveBlocks[AXIS_GAMMA].TargetPosition;
}

MoveBlock* Queue_MoveBlock::GetRoom(){
    MoveBlock* result = (MoveBlock*)this->_GetRoom(); 
    // Logger::Debug("Queue_MoveBlock::GetRoom()");
    // Logger::Print("Queue_MoveBlock::GetRoom()  AXIS_ALPHA position", result->MoveBlocks[AXIS_ALPHA].TargetPosition);
    return result;
}
