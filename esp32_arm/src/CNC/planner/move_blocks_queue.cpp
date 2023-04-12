#include "move_blocks_queue.h"

gs_MoveBlock_Queue::gs_MoveBlock_Queue(){
    this->_Init("MoveBlocks", MOVE_BLOCKS_QUEUE_SIZE, this->__all_move_blocks);
}

// MoveBlock* gs_MoveBlock_Queue::Withdraw(){
//     return this->_Withdraw(); 
// }

// MoveBlock* gs_MoveBlock_Queue::GetHead_MoveBlock(){
//     return this->_GetHeadObject();
// }


void gs_MoveBlock_Queue::DeepCopyHead_ToPosition(IKPosition_abgdekl* ik_position){
    MoveBlock* head_mb = this->GetDepositHeadElement();
    for (int a=0; a<CNC_ACTUATORS_IDEAL_COUNT; a++){
        ik_position->Positions[a] = head_mb->MoveBlocks[a].TargetPosition;
    }
    // ik_position->alpha = head_mb->MoveBlocks[AXIS_ALPHA].TargetPosition;
    // ik_position->beta = head_mb->MoveBlocks[AXIS_BETA].TargetPosition;
    // ik_position->gamma =head_mb->MoveBlocks[AXIS_GAMMA].TargetPosition;
}




