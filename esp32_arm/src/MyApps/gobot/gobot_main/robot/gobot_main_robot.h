#pragma once


#include "Robot/robot_base.h"
// #include "MyApps/cnc_machine/gobot_main/board/board.h"
#include "MyApps/gobot/gobot_main/board/board.h"
#include "gobot_main_g28_runner.h"
// #include "CNC/arm_solution/five_bars/five_bars_arm_solution.h"
#include "arm_solution_five_bars/five_bars_arm_solution.h"

#define CNC_ACTUATORS_COUNT 2

class GobotMainRobot: public RobotBase{
    public:
        void Init(GobotMain_Board* board);

    private:
        // void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};
        void __InitActuator(GobotMain_Board* board);
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;
        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
	    // CncActuatorStepper __actuator_alpha = CncActuatorStepper(); 
        // CncActuatorStepper __actuator_beta = CncActuatorStepper();
	    CncActuatorFastStepper __actuator_alpha;
	    CncActuatorFastStepper __actuator_beta;

        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        CncMover mover;
        FiveBars_ArmSolution arm_solution;
        GobotMain_G28_Runner g28_runner;
};