#pragma once


#include "Robot/robot_base.h"
#include "box_mover_g28_runner.h"
#include "CNC/mover/dual_stepper.h"
#include "CNC/arm_solution/core_yz/core_yz_arm_solution.h"

class BoxMoverRobot: public RobotBase{
    public:
        void Init(Board2205Cnc* board);

    private:
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};


        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        // Mover_DualStepper mover;
        MoverBase mover;
        CoreYZ_ArmSolution arm_solution;
        BoxMover_G28_Runner g28_runner;

};