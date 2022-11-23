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

        virtual bool GetCurrentPosition(FKPosition_XYZRPY* position_fk);

        // Todo:  should remove these two lines?
        virtual float GetDistanceToTarget_FK();
        virtual float GetDistanceToTarget_IK();


        virtual void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis);
        void ForceStopMover();  //TODO:  para is MoverAction:  EnableActuator, ForceStop, DisableActuator. 

        virtual bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode);
        virtual void __ConvertSegment_ToMoveBlockQueue(LineSegment* line);
        
    protected:
        virtual void IK(FKPosition_XYZRPY* from_fk, IKPosition_abgdekl* to_ik);
        virtual void FK(IKPosition_abgdekl* from_ik,FKPosition_XYZRPY* to_fk);


        bool is_absolute_position = true;

    private:

};

