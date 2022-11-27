#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "twh_arm_solution_config.h"

// class EnumQuandrant{
//     public:
//         Quandrant_XPYP,


// };

class Twh_ArmSolution: public ArmSolutionBase{
    public:
        // enum EnumQuadrant{
        //     Quadrant_XPYP,
        //     Quadrant_XPYN,
        //     Quadrant_XNYN,
        //     Quadrant_XNYP,
        // };

        void LinkConfig(Core_XY_XA_ab_config* config){this->_config=config;};

    protected:
        void IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik) override;
        void FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk) override;

        FKPosition_XYZRPY __current_fk_position;
        IKPosition_abgdekl __current_ik_position;

        Core_XY_XA_ab_config* _config;

    private:

};