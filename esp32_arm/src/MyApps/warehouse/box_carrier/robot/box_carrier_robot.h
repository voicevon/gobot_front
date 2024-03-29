#pragma once


#include "Robot/robot_base.h"
#include "box_carrier_g28_runner.h"
// #include "CNC/mover/dual_stepper.h"
#include "CNC/arm_solution/core_yz/core_yz_arm_solution.h"

class BoxCarrierRobot: public RobotBase{
    public:
        void Init(Board2204Cnc* board);
        void _InitStatic_Queues() override{};
        void _Init_ArmSolution() override{};
    private:
        // void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};


        MoveBlock __all_move_blocks[MOVE_BLOCKS_QUEUE_SIZE]; 
        LineSegment __all_line_segments[LINE_SEGMENTS_QUEUE_SIZE];
        // Mover_DualStepper mover;
        CncMover mover;
        CoreYZ_ArmSolution arm_solution;
        BoxCarrier_G28_Runner g28_runner;

};