#pragma once

#include "Robot/robot_base.h"
#include "MyApps/teeth_wh/board/teeth_wh_board.h"
#include "teeth_wh_g28_runner.h"
#include "CNC/arm_solution/core_xy_xa_ab/core_xy_xa_ab_arm_solution.h"

#define CNC_ACTUATORS_COUNT 2


class TeechWarehouse_Robot: public RobotBase{
    public:
        void Init(TeethWarehouse_Board* board);

    private:
        // void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};
        void __Init_actuators(TeethWarehouse_Board* board);

        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
	    CncActuatorDcMotor __actuator_alpha = CncActuatorDcMotor(); 
	    CncActuatorDcMotor __actuator_beta = CncActuatorDcMotor(); 
        
        CncMoverBase mover;
        CncSolution_CoreXY_XA_ab arm_solution;
        TeethWarehouse_G28_Runner g28_runner;

        Core_XY_XA_ab_config arm_config;
        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];

};