#pragma once

#include "move_block.h"
#include "MyLibs/common_queue/common_queue.h"

#define BLOCK_QUEUE_SIZE 88

class BlockQueue: public CommonQueue{
    public:
        MoveBlock* FetchTailMoveBlock(){return (MoveBlock*)this->FetchTailObject(); };
        MoveBlock* GetHeadMoveblock() {return (MoveBlock*)this->GetHeadObject(); };
    private:

};