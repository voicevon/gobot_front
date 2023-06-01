#pragma once

#include "Robot/robot_base.h"
#include "arm_soution_circle_loop/circle_loop_arm_solution.h"
#include "arm_soution_circle_loop/g28_runner.h"
#include "mcode_runner/json_loader.h"
#include "mcode_runner/twh_loop_porter_m42_switch_output.h"
#include "../board/board.h"



#define CNC_ACTUATORS_COUNT 1


class Twh_LoopPorter_Robot: public RobotBase{
    public:
        void Init(Twh_LoopPorter_Board* board, const char* mqtt_topic_for_home_position);
        void MySpinOnce();

    private:
        void __InitStatic_Actuators(Twh_LoopPorter_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;


        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
        CncActuatorFastStepper __actuator_alpha;
        
        Twh_Circleloop_ArmSolution __arm_solution;
        CncMover __mover;

        Twh_LoopPorter_G28_Runner __g28_runner;
        Twh_LoopPorter_JsonLoader __json_loader_for_m408;
        Twh_LoopPorter_M42_Runner_Switch_Output __m42_runner_led_output;

        MoveBlock __all_move_blocks[MOVE_BLOCKS_QUEUE_SIZE]; 
        LineSegment __all_line_segments[LINE_SEGMENTS_QUEUE_SIZE];

        Twh_LoopPorter_Board* __board;

        // int __current_state = 0;  //idle=0.   moving=1 , ready=2
};

