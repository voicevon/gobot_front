#pragma once
#include "CNC/solution/five_bars/cnc_five_bars_base.h"
#include "../board/board.h"
#include "solution_config.h"
#include "mover.h"

class GobotMainCncSolution: public CncSolution_FiveBarsBase{
    public:  
        void Init(CncBoardBase* board) override;
        void Init(GobotMain_Board* board, StepControl* step_control);

    private:
        // void _Init(Board_GobotMain* board);
        EnumAxis ConvertToEnum(char axis) override;
        GobotMain_CncMover __mover;
        GobotMain_CncSolutionConfig __config;
        void __SelfTest();

};