#pragma once
// #include "CNC/mover/single_dc_motor.h"
#include "../board/board_vsc.h"
#include "CNC/mover/single_axis.h" 

class Vsc_CncMover: public CncMover_SingleAxis{
    public:
        void Init(Vsc_Board* board);
        
    private:
};