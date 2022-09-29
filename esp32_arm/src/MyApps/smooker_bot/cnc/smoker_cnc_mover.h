#pragma once
#include "../board/smoker_board.h"
#include "CNC/mover/single_axis.h" 

class Smoker_CncMover: public Mover_SingleAxis{
    public:
        void Init(Smoker_Board* board);
        void PrintOut(const char* title) override;
        
    private:
};