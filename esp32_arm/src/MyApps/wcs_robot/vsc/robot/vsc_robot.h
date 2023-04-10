#pragma once

#include "../board/board_vsc.h"
#include "Robot/robot_base.h"
#include "arm_solution/vsc_arm_solution.h"
#include "MyLibs/basic/list/pid_controllers_list.h"
#include "vsc_g28_runner.h"

#define PID_CONTROLLERS_COUNT 1
#define CNC_ACTUATORS_COUNT 1
#define STORE_LAYER_COUNT 20


class VscRobot: public RobotBase{
    public:
        void Init(Vsc_Board* board);
        void MoveTo(int layer_index, int cell_index);

    private:
        void __Init_pids();
        void __Init_layer_position();
        void __Init_actuators(Vsc_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;

        PidControllers_Listable* __all_pids[PID_CONTROLLERS_COUNT];
        PidControllers_Listable __speed_pid = PidControllers_Listable(1.0f, 1.0f, 1.0f, 10.0f, 255.0f);
        // PidControllers_Listable __test_pid = PidControllers_Listable(3.0f, 4.0f, 5.0f, 33.0f, 33.0f);

        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
	    CncActuatorDcMotor __actuator_alpha = CncActuatorDcMotor(); 
        
        CncMover mover;
        Vsc_ArmSolution arm_solution;
        Vsc_G28_Runner g28_runner;


        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];


        CncActuatorDcMotor* __motor;
        int __position_in_pitch[STORE_LAYER_COUNT];
        int __GEAR_TEETH = 56;
        int __PITCH_COUNT = 157;
};