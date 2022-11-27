#pragma once


#include "Robot/robot_base.h"
#include "MyApps/smooker_bot/board/smoker_board.h"
#include "smoker_g28_runner.h"
#include "arm_solution_circle_loop/circle_loop_arm_solution.h"

class SmokerRobot: public RobotBase{
    public:
        void Init(Smoker_Board* board);

    private:
        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        CncMover mover;
        Smoker_ArmSolution arm_solution;
        Smoker_G28_Runner g28_runner;

};