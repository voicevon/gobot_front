#pragma once


#include "Robot/robot_base.h"
#include "MyApps/spring_maker/board/board.h"
#include "spring_maker_g28_runner.h"
#include "CNC/arm_solution/single_x_a/single_x_a_arm_solution.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"


#define CNC_ACTUATORS_COUNT 1

class SpringMakerRobot: public RobotBase{
    public:
        void Init(Board_SpringMaker* board);

    private:
        // void RunM123(uint8_t eef_channel, uint8_t eef_action) override{};
        void _InitStatic_Queues() override;
        void _Init_ArmSolution() override;
        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
	    CncActuatorStepper __actuator_alpha = CncActuatorStepper(); 
        // Mover_SingleAxis mover;
        CncMover mover;
        SingleAxis_ArmSolution arm_solution;
        SpringMaker_G28_Runner g28_runner;


        MoveBlock __all_move_blocks[88]; 
        LineSegment __all_line_segments[88];
        // Spring_CncMover mover;

};