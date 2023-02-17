#pragma once

#include "Robot/robot_base.h"
#include "arm_soution_pump/sillicon_pump_arm_solution.h"
#include "arm_soution_pump/g28_runner.h"
#include "mcode_runner/json_loader.h"
#include "mcode_runner/sillicon_pump_m42_switch_output.h"
#include "../board/board.h"

#define QUEUE_LINE_SEGMENT_COUNT 1
#define QUEUE_PLANNER_BLOCK_COUNT 1

#define CNC_ACTUATORS_COUNT 3


class Sillicon_Pump_Robot: public RobotBase{
    public:
        void Init(Sillicon_Pump_Board* board);
        void MySpinOnce();

    private:
        void __InitStatic_Actuators(Sillicon_Pump_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;


        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
        CncActuatorFastStepper __actuator_alpha;
        CncActuatorFastStepper __actuator_beta;
        CncActuatorFastStepper __actuator_gamma;
        
        // Twh_Circleloop_ArmSolution __arm_solution;
        Sillicon_Pump_ArmSolution __arm_solution;
        CncMover __mover;

        // Twh_LoopPorter_G28_Runner __g28_runner;
        Sillicon_Pump_G28_Runner __g28_runner;
        // Twh_LoopPorter_JsonLoader __json_loader_for_m408;
        Sillicon_Pump_JsonLoader __json_loader_for_m408;
        // Twh_LoopPorter_M42_Runner_Switch_Output __m42_runner_led_output;
        Sillicon_Pump_M42_Runner_Switch_Output __m42_runner_led_output;

        MoveBlock __all_move_blocks[QUEUE_PLANNER_BLOCK_COUNT]; 
        LineSegment __all_line_segments[QUEUE_LINE_SEGMENT_COUNT];

        Sillicon_Pump_Board* __board;

        // int __current_state = 0;  //idle=0.   moving=1 , ready=2
};

