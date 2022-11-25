#pragma once


#include "Robot/robot_base.h"
#include "MyApps/gobot_main/board/board.h"
#include "gobot_house_g28_runner.h"
#include "CNC/arm_solution/scara_xy_ab/scara_xy_ab_arm_solution.h"

class GobotHouseRobot: public RobotBase{
    public:
        void Init(GobotMain_Board* board);

    private:
        // void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};

        // PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        // PidControllers __all_pids = PidControllers(1);

        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        CncMover  mover;
        Scara_ArmSolution arm_solution;
        GobotHouse_G28_Runner g28_runner;

};