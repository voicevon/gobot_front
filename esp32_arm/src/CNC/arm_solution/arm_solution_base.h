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
        virtual void IK(FKPosition_XYZRPY* from_fk, IKPosition_abgdekl* to_ik);
        virtual void FK(IKPosition_abgdekl* from_ik,FKPosition_XYZRPY* to_fk);
        virtual void MK_to_FK(FKPosition_XYZRPY* from_mk, FKPosition_XYZRPY* to_fk){};
        virtual void MK_to_Ik(FKPosition_XYZRPY* from_mk, IKPosition_abgdekl* to_ik){};

        void ForceStopMover();  //TODO:  para is MoverAction:  EnableActuator, ForceStop, DisableActuator. 

        void SetCurrentPositionAs(FKPosition_XYZRPY* fk_position);
        void SetCurrentPositionAs(IKPosition_abgdekl* ik_position);
        void GetRealTimePosition(FKPosition_XYZRPY* position_fk);
        void Test(FKPosition_XYZRPY* input);
        
    protected:

    private:
        FKPosition_XYZRPY __current_position_fk;
        IKPosition_abgdekl __current_position_ik;
};
