#pragma once

#include "Robot/robot_base.h"
#include "../board/twh4_ship_out_board.h"
#include "mcode_runner/json_loader.h"
#include "mcode_runner/twh_m42_switch_output.h"
#include "arm_soution_blank/blank_arm_solution.h"
#include "arm_soution_blank/g28_runner.h"

#define QUEUE_LINE_SEGMENT_COUNT 1
#define QUEUE_PLANNER_BLOCK_COUNT 1

#define CNC_ACTUATORS_COUNT 0


class Twh4_ShipOut_Robot: public RobotBase{
    public:
        void Init(Twh4_ShipOut_Board* board);
        void MySpinOnce();  // ??

    private:
        int __current_state = 0;  //idle=0.   moving=1 , ready=2
        Twh4_JsonLoader __json_loader;

        Blank_ArmSolution __arm_solution;
        CncMover __mover;
        Twh4_Shipout_G28_Runner __g28_runner;
        Twh4_Shipout_M42_Runner_Switch_Output __m42_runner;
        
        MoveBlock __all_move_blocks[QUEUE_PLANNER_BLOCK_COUNT]; 
        LineSegment __all_line_segments[QUEUE_LINE_SEGMENT_COUNT];

        // useless, because I have no mover. no armsolution. ??
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;
        Twh4_ShipOut_Board* __board;

};

