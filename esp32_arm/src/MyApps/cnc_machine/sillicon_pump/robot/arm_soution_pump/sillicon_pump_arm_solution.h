#pragma once

#include "MyLibs/board/board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/planner/planner.h"
// #include "circle_loop_arm_solution_config.h"
#include "sillicon_pump_arm_solution_config.h"

class Sillicon_Pump_ArmSolution: public ArmSolutionBase{
    public:
        Sillicon_Pump_ArmSolution();
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;
    private:
        float __slope;
};