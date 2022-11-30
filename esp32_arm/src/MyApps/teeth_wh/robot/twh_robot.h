#pragma once

#include "Robot/robot_base.h"
#include "MyApps/teeth_wh/board/teeth_wh_board.h"
// #include "CNC/arm_solution/core_xy_xa_ab/core_xy_xa_ab_arm_solution.h"
#include "twh_arm_solution.h"
#include "twh_g28_runner.h"
#include "MyApps/teeth_wh/board/eef/teeth_wh_eef.h"

#define ROWS_COUNT 20
#define COLS_COUNT 8

#define QUEUE_LINE_SEGMENT_COUNT 5
#define QUEUE_PLANNER_BLOCK_COUNT 88

#define CNC_ACTUATORS_COUNT 3


class TeechWarehouse_Robot: public RobotBase{
    public:
        void Init(TeethWarehouse_Board* board);

    private:
        void __InitStatic_Actuators(TeethWarehouse_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;
        void _Convert_MK_to_FK(LineSegment * middle_kinematic_line, LineSegment* fk_line) override;


        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
	    // CncActuatorStepper __actuator_alpha;
	    // CncActuatorStepper __actuator_beta;
        CncActuatorFastStepper __actuator_alpha;
        CncActuatorFastStepper __actuator_beta;
        CncActuatorServo __actuator_gamma;
        
        CncMover __mover;
        Twh_ArmSolution __arm_solution;
        Twh_G28_Runner __g28_runner;

        Core_XY_XA_ab_config __arm_config;
        MoveBlock __all_move_blocks[QUEUE_PLANNER_BLOCK_COUNT]; 
        LineSegment __all_line_segments[QUEUE_LINE_SEGMENT_COUNT];

        float __GetWeight();
        void __GetDistanceToTeeth();

};