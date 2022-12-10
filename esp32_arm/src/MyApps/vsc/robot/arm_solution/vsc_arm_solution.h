#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
// #include "circle_loop_arm_solution_config.h"
#include "vsc_arm_solution.h"
#include "CNC/planner/planner.h"

class Vsc_ArmSolution: public ArmSolutionBase{
    public:
        Vsc_ArmSolution();

    private:
        void FK_to_IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPY*  to_fk) override;
        float __sensor_slope;
        float __sensor_offset;
};