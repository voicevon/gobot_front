#pragma once

#include "MyLibs/board/board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/planner/planner.h"
#include "circle_loop_arm_solution_config.h"

class Twh2_Circleloop_ArmSolution: public ArmSolutionBase{
    public:
        Twh2_Circleloop_ArmSolution();
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;
        // void RecalculateOffset();
    private:
        float __slope;
        // float __offset;
};