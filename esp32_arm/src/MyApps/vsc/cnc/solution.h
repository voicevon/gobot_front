#pragma once
#include "../board/board_vsc.h"
#include "mover.h"
#include "CNC/arm_solution/axis_homer/cnc_homers.h"
#include "CNC/arm_solution/circle_loop/cnc_circle_loop.h"

class Vsc_CncSoution: public CncCircleLoop{
    public:
        Vsc_CncSoution(){};
        EnumAxis ConvertToEnum(char axis) override;
        void Init(Vsc_Board* board);

    private:
        Vsc_CncMover __mover;




};