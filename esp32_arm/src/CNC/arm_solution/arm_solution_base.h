#pragma once

#include "../coordinate/coordinate_base.h"
#include "../coordinate/cnc_axis.h"
#include "../mover/cnc_mover_base.h"
#include "CNC/gcode/Gcode.h"

// enum class MoverState{    //TODO: rename to MoverState?
//     IDLE,       // Motor is enabled.
//     SLEEPING,   // Motor is disabled
//     RUNNING_G1,
// };

class ArmSolutionBase{
    public:
        virtual void FK_to_IK(FKPosition_XYZRPW* from_fk, IKPosition_abgdekl* to_ik);
        virtual void IK_to_FK(IKPosition_abgdekl* from_ik,FKPosition_XYZRPW* to_fk);
        virtual void MK_to_FK(FKPosition_XYZRPW* from_mk, FKPosition_XYZRPW* to_fk){};
        virtual void MK_to_Ik(FKPosition_XYZRPW* from_mk, IKPosition_abgdekl* to_ik){};

        void ForceStopMover();  //TODO:  para is MoverAction:  EnableActuator, ForceStop, DisableActuator. 

        void SetCurrentPositionAs(FKPosition_XYZRPW* fk_position);
        void SetCurrentPositionAs(IKPosition_abgdekl* ik_position);
        void GetRealTimePosition(FKPosition_XYZRPW* position_fk);
        // This is mainly for planner. TODO:  move to planner?
        FKPosition_XYZRPW* GetCurrentPosition_Fk(){return &__current_position_fk;};
        void Test(FKPosition_XYZRPW* input);
        
    protected:

    private:
        FKPosition_XYZRPW __current_position_fk;
        IKPosition_abgdekl __current_position_ik;
};
