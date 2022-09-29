#pragma once

#include "move_block.h"
#include "MyLibs/common_queue/common_queue.h"

#define BLOCK_QUEUE_SIZE 88


//TODO:  want a better name, like  MoveBlockQueueManager ?
class Queue_MoveBlock: public CommonQueue{
    public:
        static Queue_MoveBlock& Instance(){
            static Queue_MoveBlock instance;
            return instance;
        };
        Queue_MoveBlock(){this->_Init(BLOCK_QUEUE_SIZE, sizeof(MoveBlock));};
        MoveBlock* FetchTailMoveBlock(){return (MoveBlock*)this->FetchTailObject(); };
        MoveBlock* GetHeadMoveblock() {return (MoveBlock*)this->GetHeadObject(); };
    private:
        

};