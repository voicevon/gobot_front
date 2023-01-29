#pragma once

#include "Robot/robot_base.h"
#include "arm_soution_circle_loop/circle_loop_arm_solution.h"
#include "arm_soution_circle_loop/g28_runner.h"
#include "mcode_runner/json_loader.h"
#include "mcode_runner/twh_m42_switch_output.h"
#include "../board/twh2_row_board.h"

#define QUEUE_LINE_SEGMENT_COUNT 1
#define QUEUE_PLANNER_BLOCK_COUNT 1

#define CNC_ACTUATORS_COUNT 1


class Twh2Row_Robot: public RobotBase{
    public:
        void Init(Twh2Row_Board* board);
        void MySpinOnce();

    private:
        void __InitStatic_Actuators(Twh2Row_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;


        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
        CncActuatorFastStepper __actuator_alpha;
        
        Twh2_Circleloop_ArmSolution __arm_solution;
        CncMover __mover;

        Twh2Row_G28_Runner __g28_runner;
        Twh2_Row_JsonLoader __json_loader_for_m408;
        Twh2Row_M42_Runner_Switch_Output __m42_runner_led_output;

        MoveBlock __all_move_blocks[QUEUE_PLANNER_BLOCK_COUNT]; 
        LineSegment __all_line_segments[QUEUE_LINE_SEGMENT_COUNT];

        // int __current_state = 0;  //idle=0.   moving=1 , ready=2

};

