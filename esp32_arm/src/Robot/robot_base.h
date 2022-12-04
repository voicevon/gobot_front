#pragma once

#include "CNC/gcode/gcode_consumer.h"
#include "CNC/coordinate/coordinate_base.h"
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/planner/planner.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/mover/cnc_mover_base.h"
#include "Robot/gcode_runner/g28_runner.h"
#include "Robot/gcode_runner/g4_runner.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "Robot/eef/robot_eef_base.h"

enum class RobotState{
    IDLE_OR_ASYNC,
    G28_IS_SYNCING,  //Wait command in queue in front of G28 is Done.
    G28_IS_RUNNING,
    G4_IS_SYNCING,   //Wait command in queue in front of G4 is Done.
    G4_IS_RUNNING,
    MCODE_IS_SYNCING,
    MCODE_IS_RUNNING,
};


class RobotBase: public GcodeConsumer{
    public:
        RobotState State = RobotState::IDLE_OR_ASYNC;
        void SpinOnce();

    protected:
        void _running_G28();
        void _LinkMover(CncMover* mover){this->_mover=mover;};
        void _LinkArmSolution_for_planner(ArmSolutionBase* arm_solution){__planner.arm_solution=arm_solution;};
        virtual void _InitStatic_Queues();
        virtual void _Init_ArmSolution();
        
        G28_Runner* _g28_runner;
        CncMover* _mover;

    private:
        Planner __planner;
        G4_Runner __g4_runner;
        void __RunGcode(Gcode* gcode);

        

        /* Just for fun, don't remove below comment.
        void * __output_message2;
        void (* __output_message3); 
        void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        */

};

