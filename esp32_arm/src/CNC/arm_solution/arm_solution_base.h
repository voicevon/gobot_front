#pragma once

#include "MyLibs/pid_controllers/pid_controllers.h"
#include "CNC/board/cnc_board_base.h"
#include "CNC/gcode/gcode_consumer.h"
#include "kinematic_config.h"
#include "../coordinate/coordinate_base.h"
#include "../coordinate/cnc_axis.h"
#include "../mover/mover_base.h"
#include "Robot/axis_homer/axis_homer.h"
#include "Robot/axis_homer/homer_diction.h"
#include "Robot/eef/eef_standard_code.h"

// enum class MoverState{    //TODO: rename to MoverState?
//     IDLE,       // Motor is enabled.
//     SLEEPING,   // Motor is disabled
//     RUNNING_G1,
// };

class ArmSolutionBase{
    public:

        virtual bool GetCurrentPosition(FkPositionBase* position_fk);

        // Todo:  should remove these two lines?
        virtual float GetDistanceToTarget_FK();
        virtual float GetDistanceToTarget_IK();


        void SayHello();    // TODO:  be virtual
        virtual void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis);
        void ForceStopMover();
        CncBoardBase* _cnc_board;   // TODO:  remove this.

        virtual bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode);
        virtual void __ConvertSegment_ToMoveBlockQueue(LineSegment* line);
        
    protected:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik);
        virtual void FK(IkPositionBase* from_ik,FkPositionBase* to_fk);
        virtual std::string GetHomeTrigerStateString();  //TODO:: remove-able?

        void Run_M42_OutputGpio(uint8_t pin_number, uint8_t pin_value);  //TODO:: remove-able?

        KinematicConfig _config_base;    //TODO:  rename to _kinamatic_config
        HomerDiction _homer_diction = HomerDiction(CNC_AXIS_COUNT);

        bool is_absolute_position = true;

    private:

};

