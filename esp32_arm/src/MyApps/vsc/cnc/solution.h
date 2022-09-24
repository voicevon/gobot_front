#pragma once
#include "CNC/solution/single_axis/cnc_single_axis.h"
#include "../board/board_vsc.h"
#include "mover.h"
#include "CNC/solution/axis_homer/cnc_homers.h"

class Vsc_CncSoution: public CncSingleAxis{
    public:
        Vsc_CncSoution();
        EnumAxis ConvertToEnum(char axis) override;
        void Init(Vsc_Board* board);

    private:
        Vsc_CncMover __mover;

};