#pragma once


#include "CNC/arm_solution/arm_solution_base.h"
#include "config_base.h"
#include "MyLibs/MyFunctions.hpp"


class FiveBars_ArmSolution: public ArmSolutionBase{
    public:

    protected:

    
    private:
        virtual void IK(FKPosition_XYZRPY* from_fk, IKPosition_abgdekl* to_ik) override;
        virtual void FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY* to_fk) override;

        // FKPosition_XYZRPY __current_fk_position;
        CncSolution_FiveBarConfig* _config;  //TODO:: remove-able?


};


