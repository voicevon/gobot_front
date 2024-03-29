#pragma once


#include "CNC/arm_solution/arm_solution_base.h"
#include "config_base.h"
#include "MyLibs/MyFunctions.hpp"


class FiveBars_ArmSolution: public ArmSolutionBase{
    public:

    protected:

    
    private:
        virtual void FK_to_IK(FKPosition_XYZRPW* from_fk, IKPosition_abgdekl* to_ik) override;
        virtual void IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW* to_fk) override;

        // FKPosition_XYZRPW __current_fk_position;
        CncSolution_FiveBarConfig* _config;  //TODO:: remove-able?


};


