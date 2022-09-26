#pragma once

#include "block.h"

#define BLOCK_QUEUE_SIZE 88

class BlockQueue{
    public:
        // void AppendBlock(Block * block);
        Block* GetFreeBlock();
        int GetFreeBlocksCount();

    private:
        Block __all_blocks[BLOCK_QUEUE_SIZE];

};