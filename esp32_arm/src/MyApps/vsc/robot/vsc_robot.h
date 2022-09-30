#pragma once


#include "Robot/robot_base.h"
#include "MyApps/vsc/board/board_vsc.h"
#include "vsc_g28_runner.h"
#include "CNC/arm_solution/circle_loop/circle_loop_arm_solution.h"
#include "CNC/mover/single_axis.h"

class VscRobot: public RobotBase{
    public:
        void Init(Vsc_Board* board);

    private:
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};

        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        PidControllers __all_pids = PidControllers(1);

        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        // Mover_SingleDcMotor mover;
        Mover_SingleAxis mover;
        CircleLoop_ArmSolution arm_solution;
        Vsc_G28_Runner g28_runner;

};