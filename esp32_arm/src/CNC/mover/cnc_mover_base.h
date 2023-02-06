
#pragma once

#include <stdint.h>
#include "MyLibs/basic/logger.h"   // todo: remove

#include "../coordinate/cnc_axis.h"
#include "MyLibs/board/board_base.h"
#include "CNC/Actuator/cnc_actuator_list.h"
#include "FastAccelStepper.h"



class CncMover{
    public:
        // To do some move
        void SpinOnce();
        void AllActuatorsMoveTo(MoveBlock* move);
        void AllActuatorsStop(); 

        // void SetActuatorCurrentCncPositionAs(EnumAxis_Inverseinematic actuator_name, float as_current_position);

        // To get status or position
        float GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis_Inverseinematic actuator_name);  // TODO: return  all current position.
        bool ActuatorIsMoving(EnumAxis_Inverseinematic actuator_name);
        bool IsMoving();
        float GetAbsDistanceToTarget_InCncUnit();


    protected:
        // uint8_t _moving_actuator_flags;
        float _small_distance_for_arriving_target_position = 0.1;   //Todo: configable
    
    private:
        // StepControl* __stepControl;
    
};