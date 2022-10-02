#pragma once


#include "Robot/robot_base.h"
#include "MyApps/vsc/board/board_vsc.h"
#include "vsc_g28_runner.h"
#include "CNC/arm_solution/circle_loop/circle_loop_arm_solution.h"
#include "CNC/mover/single_axis.h"
#include "MyLibs/list/pid_controllers_list.h"
// #include "MyLibs/array/pid_controllers_array.h"


#define PID_CONTROLLERS_COUNT 1
#define CNC_ACTUATOR_COUNT 1


class VscRobot: public RobotBase{
    public:
        void Init(Vsc_Board* board);

    private:
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};

        PidControllers_Listable __speed_pid = PidControllers_Listable(1.0f, 1.0f, 0.0f, 10.0f, 255.0f);
        PidControllers_Listable** __all_pids;
        // PidControllers_Listable __all_pid_controllers[PID_CONTROLLERS_COUNT];

        ActuatorBase* __all_actuators[CNC_ACTUATOR_COUNT];

	    ActuatorDcMotor __actuator_alpha = ActuatorDcMotor(); 
        
        Mover_SingleAxis mover;
        CircleLoop_ArmSolution arm_solution;
        Vsc_G28_Runner g28_runner;


        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];

};