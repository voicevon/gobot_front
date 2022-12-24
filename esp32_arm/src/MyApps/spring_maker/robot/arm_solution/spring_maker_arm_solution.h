#pragma once

#include "CNC/arm_solution/arm_solution_base.h"

class SpringMaker_ArmSolution: public ArmSolutionBase{
    public:


    private:
        virtual void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        virtual void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;

};