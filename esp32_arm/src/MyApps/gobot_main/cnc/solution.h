#pragma once
#include "CNC/solution/five_bars/cnc_five_bars_base.h"
#include "../board/board.h"
// #include "CNC/mover/dual_stepper.h"
#include "solution_config.h"
#include "mover.h"

class GobotMainCnc: public CncSolution_FiveBarsBase{
    public:  
        void Init(CncBoardBase* board) override;

    private:
        // void _Init(Board_GobotMain* board);
        EnumAxis ConvertToEnum(char axis) override;
        Gobot_CncMover __mover;
        GobotMain_CncSolutionConfig __config;
        void __SelfTest();

};