#include "move_block.h"


void MoveBlock::DeepCopyTo(Queue_able* copy){
    MoveBlock* cp = (MoveBlock*) copy;
    cp->axis = this->axis;
    cp->Acceleration = this->Acceleration;

}
