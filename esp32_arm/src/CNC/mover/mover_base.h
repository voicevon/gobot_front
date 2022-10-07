
#pragma once

#include <stdint.h>
#include "MyLibs/basic/logger.h"

#include "../coordinate/cnc_axis.h"
#include "CNC/board/cnc_board_base.h"
#include "CNC/Actuator/cnc_actuator_list.h"



class MoverBase{
    public:
        // To do some move
        void SpinOnce();
        void AllActuatorsMoveTo(MoveBlock* move);
        void AllActuatorsStop(); 

        void SetActuatorCurrentCncPositionAs(EnumAxis_Inverseinematic actuator_name, float as_current_position);

        // To get status or position
        float GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis_Inverseinematic actuator_name);  // TODO: return  all current position.
        void SetMovingFlags(uint8_t target_actuator_flags){this->_moving_actuator_flags=target_actuator_flags;};
        // virtual bool ActuatorIsMoving(EnumAxis_Inverseinematic actuator_name);
        bool ActuatorIsMoving(EnumAxis_Inverseinematic actuator_name);
        // virtual bool HasArrivedTargetPosition();
        bool HasArrivedTargetPosition();
        float GetAbsDistanceToTarget_InCncUnit();


    protected:
        uint8_t _moving_actuator_flags;
        float _small_distance_for_arriving_target_position = 0.1;   //Todo: configer-able
    
    private:
        StepControl* __stepControl;
    
};