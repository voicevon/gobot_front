
#pragma once

#include <stdint.h>
#include "CNC/Actuator/cnc_actuator_list.h"
#include "../coordinate/cnc_axis.h"
#include "MyLibs/basic/logger.h"   // todo: remove
#include "MyLibs/board/board_base.h"
#include "FastAccelStepper.h"



class CncMover{
    public:
        // To do some move
        void SpinOnce();
        void AllActuatorsMoveTo(MoveBlock* move);
        void AllActuatorsStop(); 
        void AllActuator_SetPositionAs(IKPosition_abgdekl* new_position);

        // To get status or position
        bool ActuatorIsMoving(EnumAxis_Inverseinematic actuator_name);
        bool IsMoving();


    protected:
        // uint8_t _moving_actuator_flags;
        float _small_distance_for_arriving_target_position = 0.1;   //Todo: configable
    
    private:
        // StepControl* __stepControl;
    
};