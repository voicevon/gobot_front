#pragma once

#include "Robot/robot_base.h"
#include "MyApps/teeth_wh/board/teeth_wh_board.h"
#include "teeth_wh_g28_runner.h"
#include "CNC/arm_solution/core_xy_ab/core_xy_ab_arm_solution.h"
#include "MyLibs/list/pid_controllers_list.h"

#define PID_CONTROLLERS_COUNT 1
#define CNC_ACTUATORS_COUNT 1


class TeechWarehouse_Robot: public RobotBase{
    public:
        void Init(TeethWarehouse_Board* board);

    private:
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};
        void __Init_pids();
        void __Init_actuators(TeethWarehouse_Board* board);

        PidControllers_Listable* __all_pids[PID_CONTROLLERS_COUNT];
        PidControllers_Listable __speed_pid = PidControllers_Listable(1.0f, 1.0f, 1.0f, 10.0f, 255.0f);
        // PidControllers_Listable __test_pid = PidControllers_Listable(3.0f, 4.0f, 5.0f, 33.0f, 33.0f);

        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
	    CncActuatorDcMotor __actuator_alpha = CncActuatorDcMotor(); 
        
        CncMoverBase mover;
        CncSolution_CoreXY_ab arm_solution;
        TeethWarehouse_G28_Runner g28_runner;


        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];

};