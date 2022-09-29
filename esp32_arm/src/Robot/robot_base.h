#pragma once

#include "Robot/eef/eef_standard_code.h"
#include "CNC/gcode/gcode_consumer.h"
#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/kinematic_config.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/coordinate/coordinate_base.h"
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/mover/mover_base.h"
#include "CNC/planner/planner.h"
#include "gcode_runner/g28_runner.h"
#include "gcode_runner/g4_runner.h"
#include "mcode_runner/mcode_runners.h"

enum class RobotState{
    IDLE_OR_ASYNC,
    G28_IS_SYNCING,
    G28_IS_RUNNING,
    G4_IS_SYNCING,
    G4_IS_RUNNING,
};

class RobotBase: public GcodeConsumer{
    public:
        RobotState State = RobotState::IDLE_OR_ASYNC;
        void SpinOnce();
        // void RunG28(EnumAxis axis);

    protected:
        // Queue_MoveBlock __queue_move_block;
        Planner __planner;
        void _LinkEef(RobotEefBase* eef){this->__eef=eef;};
       
        // virtual std::string GetHomeTrigerStateString();

        void Run_M42_OutputGpio(uint8_t pin_number, uint8_t pin_value);
        
        virtual void RunM123(uint8_t eef_channel, uint8_t eef_action);

        void _running_G28();

        EnumAxis _homing_axis;
        KinematicConfig _config_base;    //TODO:  rename to _kinamatic_config
        CncHomers _cnc_homer = CncHomers(CNC_AXIS_COUNT);

        bool is_absolute_position = true;
        
        CncBoardBase* _cnc_board;  //!!!!
        ArmSolutionBase* _arm_solution;
        MoverBase* _mover;

    private:

        int test_int;
        RobotEefBase* __eef;
        G28_Runner __g28_runner;
        G4_Runner __g4_runner;

        void __RunGcode(Gcode* gcode);
        void __RunMcode(Gcode* gcode);

        /* Just for fun, don't remove below comment.
        void * __output_message2;
        void (* __output_message3); 
        void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        */

};

