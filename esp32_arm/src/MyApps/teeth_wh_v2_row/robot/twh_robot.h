#pragma once

#include "Robot/robot_base.h"
#include "arm_soution_circle_loop/circle_loop_arm_solution.h"
#include "arm_soution_circle_loop/twh_g28_runner.h"
#include "gmcode_runner/twh_m42_switch_output.h"
#include "MyApps/teeth_wh_v2/board/teeth_wh_board.h"

#define ROWS_COUNT 20
#define COLS_COUNT 8

#define QUEUE_LINE_SEGMENT_COUNT 5
#define QUEUE_PLANNER_BLOCK_COUNT 88

#define CNC_ACTUATORS_COUNT 3


class Twh2Row_Robot: public RobotBase{
    public:
        void Init(Twh2_Board* board);

    private:
        void __InitStatic_Actuators(Twh2_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;


        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
        CncActuatorFastStepper __actuator_alpha;
        CncActuatorFastStepper __actuator_beta;
        // CncActuatorServo __actuator_gamma;
        
        Twh2_Circleloop_ArmSolution __arm_solution;
        CncMover __mover;
        Twh2Row_G28_Runner __g28_runner;
        Twh2Row_M42_Runner_Switch_Output __m42_runner;
        

        // Twh_ArmSolution_Config __arm_config;
        MoveBlock __all_move_blocks[QUEUE_PLANNER_BLOCK_COUNT]; 
        LineSegment __all_line_segments[QUEUE_LINE_SEGMENT_COUNT];

        // float __GetWeight();
        // void __GetDistanceToTeeth();

};