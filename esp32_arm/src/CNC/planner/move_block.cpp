#include "move_block.h"



// void MoveBlock::DeepCopyTo(Queue_able* copy){
void MoveBlock::DeepCopyTo(MoveBlock* copy){
    MoveBlock* cp = (MoveBlock*) copy;
    // TODO: any risk of mem_copy ?
    for(int a=0; a<CNC_AXIS_COUNT; a++){
        // cp->MoveBlocks[a].axis = this->MoveBlocks[a].axis;
        cp->MoveBlocks[a].Acceleration = this->MoveBlocks[a].Acceleration;
        cp->MoveBlocks[a].Speed = this->MoveBlocks[a].Speed;
        // cp->MoveBlocks[a].IsAbsTargetPosition = this->MoveBlocks[a].IsAbsTargetPosition;
        cp->MoveBlocks[a].TargetPosition = this->MoveBlocks[a].TargetPosition;
    }
}

void MoveBlock::DeepReset_ToDefault(){
    // Logger::Debug("MoveBlock::DeepReset_ToDefault()");
    for(int a=0; a<CNC_AXIS_COUNT; a++){
        // Logger::Print("for  i", i );
        this->MoveBlocks[a].IsAbsTargetPosition = false;
        this->MoveBlocks[a].TargetPosition = 0;
        this->MoveBlocks[a].Speed = 0;
    }
}

