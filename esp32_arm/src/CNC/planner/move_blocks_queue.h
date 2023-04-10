#pragma once

#include "move_block.h"
#include "MyLibs/common_queue/common_queue.h"
#include "CNC/coordinate/coordinate_base.h"

#define MOVE_BLOCKS_QUEUE_SIZE 32


class gs_MoveBlock_Queue: public CommonQueue{
    public:
        static gs_MoveBlock_Queue& Instance(){
            static gs_MoveBlock_Queue instance;
            return instance;
        };
        gs_MoveBlock_Queue(){this->_Init("MoveBlocks", MOVE_BLOCKS_QUEUE_SIZE, sizeof(MoveBlock));};
        MoveBlock* Withdraw(){return (MoveBlock*)this->_Withdraw(); };
        MoveBlock* GetRoom();
        MoveBlock* GetHead_MoveBlock(){return (MoveBlock*)this->_GetHeadObject();};
        void DeepCopyHead_ToPosition(IKPosition_abgdekl* ik_position);

    private:

};