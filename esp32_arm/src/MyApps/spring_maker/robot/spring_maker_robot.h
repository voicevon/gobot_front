#pragma once


#include "Robot/robot_base.h"
#include "MyApps/spring_maker/board/board.h"
// #include "MyApps/spring_maker/cnc/spring_maker_arm_solution.h"
#include "spring_maker_g28_runner.h"
#include "CNC/arm_solution/single_axis/single_axis_arm_solution.h"

class SpringMakerRobot: public RobotBase{
    public:
        void Init(Board_SpringMaker* board);

    private:
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};

        PIDController __speed_pid = PIDController(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        PidControllers __all_pids = PidControllers(1);

        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        // Spring_CncMover mover;
        SingleAxis_ArmSolution arm_solution;
        SpringMaker_G28_Runner g28_runner;

};