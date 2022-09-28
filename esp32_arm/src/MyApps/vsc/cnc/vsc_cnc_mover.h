#pragma once
#include "../board/board_vsc.h"
#include "CNC/mover/single_axis.h" 

class Vsc_CncMover: public Mover_SingleAxis{
    public:
        void Init(Vsc_Board* board);
        void PrintOut(const char* title) override;
        
    private:
};