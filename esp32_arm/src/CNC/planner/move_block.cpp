#include "move_block.h"


void MoveBlock::DeepCopyTo(Queue_able* copy){
    MoveBlock* cp = (MoveBlock*) copy;
    // TODO: any risk of mem_copy ?
    for(int i=0; i<6; i++){
        cp->MoveBlocks[i].axis = this->MoveBlocks[i].axis;
        cp->MoveBlocks[i].Acceleration = this->MoveBlocks[i].Acceleration;
    }

}

void MoveBlock::DeepReset_ToDefault(){
        for(int i=0; i<6; i++){
        // this->MoveBlocks[i].axis = i;
        this->MoveBlocks[i].IsAbsTargetPosition = false;
        this->MoveBlocks[i].TargetPosition = 0;
        this->MoveBlocks[i].Speed = 0;
    }
}

