#pragma once
#include "../board/board_vsc.h"
#include "mover.h"
#include "CNC/arm_solution/axis_homer/cnc_homers.h"
#include "CNC/arm_solution/circle_loop/cnc_circle_loop.h"

class Vsc_CncSoution: public CncCircleLoop{
    public:
        Vsc_CncSoution();
        EnumAxis ConvertToEnum(char axis) override;
        void Init(Vsc_Board* board);

    private:
        Vsc_CncMover __mover;
        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        PidControllers __all_pids = PidControllers(1);



};