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
        void ForceStopMover();  //TODO:  para is MoverAction:  EnableActuator, ForceStop, DisableActuator. 

        void SetCurrentPosition(FKPosition_XYZRPY* fk_position);
        void SetCurrentPosition(IKPosition_abgdekl* ik_position);
        void GetRealTimePosition(FKPosition_XYZRPY* position_fk);
        
    protected:

    private:

};

