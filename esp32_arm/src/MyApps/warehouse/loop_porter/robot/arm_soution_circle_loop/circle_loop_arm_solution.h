#pragma once

#include "Robot/board/board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/planner/planner.h"
#include "circle_loop_arm_solution_config.h"

class Twh_Circleloop_ArmSolution: public ArmSolutionBase{
    public:
        Twh_Circleloop_ArmSolution();
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;
    private:
        float __slope;
};