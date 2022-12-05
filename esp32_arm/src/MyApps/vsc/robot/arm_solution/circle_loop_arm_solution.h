#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "circle_loop_arm_solution_config.h"
#include "CNC/planner/planner.h"

class CircleLoop_ArmSolution: public ArmSolutionBase{
    public:

    protected:
        CircleLoop_KinematicConfig _kinematic_config;  
        // EnumAxis_ForwardKinematic _AXIS;   // TODO: doubel check this var.

    private:
        void FK_to_IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPY*  to_fk) override;

        // FKPosition_XYZRPY __current_fk_position;
        // FKPosition_XYZRPY __target_fk_position;


};