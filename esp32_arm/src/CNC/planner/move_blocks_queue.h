#pragma once

#include "move_block.h"
#include "MyLibs/basic/queue/queue_base.h"
#include "CNC/coordinate/coordinate_base.h"

#define MOVE_BLOCKS_QUEUE_SIZE 32


class gs_MoveBlock_Queue: public QueueBase<MoveBlock>{
    public:
        static gs_MoveBlock_Queue& Instance(){
            static gs_MoveBlock_Queue instance;
            return instance;
        };
        MoveBlock* Withdraw();
        MoveBlock* GetRoom();
        MoveBlock* GetHead_MoveBlock();
        void DeepCopyHead_ToPosition(IKPosition_abgdekl* ik_position);

    private:
        gs_MoveBlock_Queue();

};