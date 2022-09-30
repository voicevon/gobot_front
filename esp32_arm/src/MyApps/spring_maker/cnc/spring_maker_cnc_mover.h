#pragma once
#include "../board/board.h"
#include "CNC/mover/single_axis.h" 

class SpringMaker_CncMover: public Mover_SingleAxis{
    public:
        void Init(Board_SpringMaker* board);
        // void PrintOut(const char* title) override;
        
    private:
};