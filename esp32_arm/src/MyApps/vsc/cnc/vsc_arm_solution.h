#pragma once
#include "../board/board_vsc.h"
#include "vsc_cnc_mover.h"
#include "CNC/arm_solution/axis_homer/cnc_homers.h"
#include "CNC/arm_solution/circle_loop/circle_loop_arm_solution.h"

class Vsc_ArmSoution: public CircleLoop_ArmSolution{
    public:
        Vsc_ArmSoution(){};
        EnumAxis ConvertToEnum(char axis) override;
        void Init(Vsc_Board* board);

    private:
        Vsc_CncMover __mover;
        Planner __planner;




};