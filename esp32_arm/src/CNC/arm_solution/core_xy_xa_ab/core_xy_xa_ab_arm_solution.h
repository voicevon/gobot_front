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

        // CncSolution_CoreXA(EnumQuadrant default_quadrant);
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override{};
        void LinkConfig(Core_XY_XA_ab_config* config){this->_config=config;};

    protected:
        void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override{};

        //Override private
        void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;
        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override{};

        FkPosition_XY __current_fk_position;
        IkPosition_AlphaBeta __current_ik_position;

        Core_XY_XA_ab_config* _config;

    private:

};