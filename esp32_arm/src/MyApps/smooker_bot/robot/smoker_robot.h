#pragma once


#include "Robot/robot_base.h"
#include "MyApps/smooker_bot/board/smoker_board.h"
#include "smoker_g28_runner.h"
#include "CNC/arm_solution/circle_loop/circle_loop_arm_solution.h"
// #include "CNC/mover/single_axis.h"

class SmokerRobot: public RobotBase{
    public:
        void Init(Smoker_Board* board);

    private:
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};

        // PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        // PidControllers __all_pids = PidControllers(1);

        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        CncMoverBase mover;
        CircleLoop_ArmSolution arm_solution;
        Smoker_G28_Runner g28_runner;

};