#pragma once

#include "MyLibs/basic/board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"

class Blank_ArmSolution: public ArmSolutionBase{
    public:
        Blank_ArmSolution();
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;

};