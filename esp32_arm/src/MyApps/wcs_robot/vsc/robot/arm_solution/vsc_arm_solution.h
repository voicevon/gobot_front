#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "vsc_arm_solution.h"
#include "CNC/planner/planner.h"

class Vsc_ArmSolution: public ArmSolutionBase{
    public:
        Vsc_ArmSolution();

    private:
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;
        float __sensor_slope;
        float __sensor_offset;
};