#pragma once

#include "Robot/eef/eef_standard_code.h"
#include "CNC/gcode/gcode_consumer.h"
#include "CNC/board/cnc_board_base.h"

#include "CNC/arm_solution/kinematic_config.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/coordinate/coordinate_base.h"
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/mover/cnc_mover_base.h"
#include "MyLibs/pid_controllers/pid_controllers.h"
#include "CNC/gcode/line_segment_queue.h"

enum class RobotState{
    IDLE,
    RUNNING_G28
};

class RobotBase: public GcodeConsumer{
    public:
        RobotState State = RobotState::IDLE;
        void SpinOnce();

        void RunGcode(Gcode* gcode);
        void RunG28(EnumAxis axis);

    protected:
        // virtual _DispatchGcode(Gcode* gcode);
        void _LinkEef(RobotEefBase* eef){this->__eef=eef;};
        void _LinkPidControllers_M130(PidControllers* pid_controllers){this->__pid_controllers_m130=pid_controllers;};
       
        // virtual std::string GetHomeTrigerStateString();

        void Run_M42_OutputGpio(uint8_t pin_number, uint8_t pin_value);
        
        void RunM84();
        virtual void RunM123(uint8_t eef_channel, uint8_t eef_action);

        void _running_G28();
		// virtual void _RunG28_CombinedFk(EnumAxis axis){};

        EnumAxis _homing_axis;
        KinematicConfig _config_base;    //TODO:  rename to _kinamatic_config
        CncHomers _cnc_homer = CncHomers(CNC_AXIS_COUNT);

        bool is_absolute_position = true;
        
        CncBoardBase* _cnc_board;  //!!!!
        // CncMoverBase* _mover_base;   //???
        LineSegmentQueue* _line_segment_queue;

        ArmSolutionBase* _arm_solution;

    private:
        void __TryNextGmCode_FromQueue();

        int test_int;
        RobotEefBase* __eef;
        // Why it's a pointer, not a object? Because I don't know how many pid controllers in the system. I can't init it.
        // And the author doesn't want to use "new PidControll()"  to create an instance.
        PidControllers* __pid_controllers_m130;   
        void _base_spin_once();
        void __HomeSingleAxis(EnumAxis axis);

        /* Just for fun, don't remove below comment.
        void * __output_message2;
        void (* __output_message3); 
        void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        */

};

