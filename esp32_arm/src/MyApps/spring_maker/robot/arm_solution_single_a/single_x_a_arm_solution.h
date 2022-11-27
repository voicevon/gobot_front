#pragma once

#include "CNC/arm_solution/arm_solution_base.h"

class SingleAxis_ArmSolution: public ArmSolutionBase{
    public:


    private:
        virtual void IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik) override;
        virtual void FK(IKPosition_abgdekl* ik, FKPosition_XYZRPY*  to_fk) override;

};