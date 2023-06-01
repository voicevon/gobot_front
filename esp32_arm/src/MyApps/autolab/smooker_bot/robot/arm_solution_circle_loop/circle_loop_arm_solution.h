#pragma once

#include "Robot/board/board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/planner/planner.h"
#include "circle_loop_arm_solution_config.h"

class Smoker_ArmSolution: public ArmSolutionBase{
    public:

    protected:
        CircleLoop_KinematicConfig _kinematic_config;  
        // EnumAxis_ForwardKinematic _AXIS;   // TODO: doubel check this var.

    private:
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;

        // FKPosition_XYZRPW __current_fk_position;
        // FKPosition_XYZRPW __target_fk_position;


};