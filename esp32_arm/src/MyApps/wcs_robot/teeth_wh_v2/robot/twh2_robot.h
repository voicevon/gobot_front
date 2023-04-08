#pragma once

#include "Robot/robot_base.h"
#include "arm_solution_core_xz/twh2_xz_arm_solution.h"
#include "arm_solution_core_xz/twh2_g28_runner.h"
#include "gmcode_runner/twh2_m42_switch_output.h"
#include "gmcode_runner/json_loader.h"

#include "MyApps/wcs_robot/teeth_wh_v2/board/teeth_wh_board.h"

// #define ROWS_COUNT 20
// #define COLS_COUNT 8


#define CNC_ACTUATORS_COUNT 2


class Twh2_Robot: public RobotBase{
    public:
        void Init(Twh2_Board* board);

    private:
        void __InitStatic_Actuators(Twh2_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;


        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
        CncActuatorFastStepper __actuator_alpha;
        CncActuatorFastStepper __actuator_beta;
        
        CncMover __mover;
        Twh2_XzArmSolution __arm_solution;
        Twh2_G28_Runner_CoreXz __g28_runner;
        Twh2_M42_Runner_Switch_Output __m42_runner;
        Twh2_JsonLoader __json_loader;

        // Twh_ArmSolution_Config __arm_config;
        MoveBlock __all_move_blocks[MOVE_BLOCKS_QUEUE_SIZE];    // TODO: move to base class
        LineSegment __all_line_segments[LINE_SEGMENTS_QUEUE_SIZE];  // TODO: move to base class

        float __GetWeight();
        void __GetDistanceToTeeth();

};