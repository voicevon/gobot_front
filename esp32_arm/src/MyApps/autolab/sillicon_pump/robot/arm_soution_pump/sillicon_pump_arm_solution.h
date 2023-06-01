#pragma once

#include "Robot/board/board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/planner/planner.h"
#include "sillicon_pump_arm_solution_config.h"

class Silicon_Pump_ArmSolution: public ArmSolutionBase{
    public:
        Silicon_Pump_ArmSolution();
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;
    private:
        float __slope;
};