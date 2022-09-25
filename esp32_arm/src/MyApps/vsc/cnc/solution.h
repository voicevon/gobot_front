#pragma once
// #include "CNC/solution/single_axis/cnc_single_axis.h"
#include "../board/board_vsc.h"
#include "mover.h"
#include "CNC/solution/axis_homer/cnc_homers.h"
#include "CNC/solution/circle_loop/kinematic.h"

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