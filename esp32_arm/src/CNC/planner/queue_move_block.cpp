#include "queue_move_block.h"

void Queue_MoveBlock::DeepCopyToPosition(IKPosition_abgdekl* ik_position){
    MoveBlock* head_mb = this->GetHead_MoveBlock();
    ik_position->alpha = head_mb->MoveBlocks[AXIS_ALPHA].TargetPosition;
    ik_position->beta = head_mb->MoveBlocks[AXIS_BETA].TargetPosition;
    ik_position->gamma =head_mb->MoveBlocks[AXIS_GAMMA].TargetPosition;
}
