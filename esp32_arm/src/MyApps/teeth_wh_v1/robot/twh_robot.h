#pragma once

#include "Robot/robot_base.h"
#include "twh_xa_arm_solution.h"
#include "gmcode_runner/twh_g28_runner.h"
#include "gmcode_runner/twh_m42_switch_output.h"
#include "MyApps/teeth_wh_v1/board/teeth_wh_board.h"
#include "MyApps/teeth_wh_v1/board/eef/teeth_wh_eef.h"

#define ROWS_COUNT 20
#define COLS_COUNT 8

#define QUEUE_LINE_SEGMENT_COUNT 5
#define QUEUE_PLANNER_BLOCK_COUNT 88

#define CNC_ACTUATORS_COUNT 3


class Twh1_Robot: public RobotBase{
    public:
        void Init(TeethWarehouse_Board* board);

    private:
        void __InitStatic_Actuators(TeethWarehouse_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;


        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
        CncActuatorFastStepper __actuator_alpha;
        CncActuatorFastStepper __actuator_beta;
        CncActuatorServo __actuator_gamma;
        
        CncMover __mover;
        Twh1_XaArmSolution __arm_solution;
        Twh1_G28_Runner __g28_runner;
        Twh1_M42_Runner_Switch_Output __m42_runner;

        Twh_ArmSolution_Config __arm_config;
        MoveBlock __all_move_blocks[QUEUE_PLANNER_BLOCK_COUNT]; 
        LineSegment __all_line_segments[QUEUE_LINE_SEGMENT_COUNT];

        float __GetWeight();
        void __GetDistanceToTeeth();

};