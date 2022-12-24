#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "core_xy_xa_ab_config.h"

// class EnumQuandrant{
//     public:
//         Quandrant_XPYP,


// };

class CncSolution_CoreXY_XA_ab: public ArmSolutionBase{
    public:
        // enum EnumQuadrant{
        //     Quadrant_XPYP,
        //     Quadrant_XPYN,
        //     Quadrant_XNYN,
        //     Quadrant_XNYP,
        // };

        void LinkConfig(Core_XY_XA_ab_config* config){this->_config=config;};

    protected:
        // void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override{};

        //Override private
        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk) override;
        // bool GetCurrentPosition(FKPosition_XYZRPW* position_fk) override{return false;};  
        // float GetDistanceToTarget_FK() override{return 0.0;};
        // float GetDistanceToTarget_IK() override{};

        FKPosition_XYZRPW __current_fk_position;
        IKPosition_abgdekl __current_ik_position;

        Core_XY_XA_ab_config* _config;

    private:

};