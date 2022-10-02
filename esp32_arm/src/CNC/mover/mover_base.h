
#pragma once

#include <stdint.h>
#include "../coordinate/cnc_axis.h"
#include "MyLibs/basic/logger.h"
#include "CNC/board/cnc_board_base.h"
#include "../Actuator/actuator_base.h"




// TODO:  template <actuator>
class MoverBase{
    public:
        void SpinOnce();
        void AllActuatorsMoveTo(MoveBlock* move);

        void LinkActuator(char actuator_name, ActuatorBase* actuator);
        void SetActuatorCurrentCncPositionAs(EnumAxis_Inverseinematic actuator_name, float as_current_position);
        float GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis_Inverseinematic actuator_name);  // TODO: return  all current position.
        void SetMovingFlags(uint8_t target_actuator_flags){this->_moving_actuator_flags=target_actuator_flags;};
        virtual void AllActuatorsStop();  // TODO: with flags
        virtual bool ActuatorIsMoving(EnumAxis_Inverseinematic actuator_name);
        virtual bool HasArrivedTargetPosition();
        float GetAbsDistanceToTarget_InCncUnit();


    protected:
        uint8_t _moving_actuator_flags;
        float _small_distance_for_arriving_target_position = 0.1;
        
        //TODO:  put all to a diction.
        ActuatorBase* _actuator_alpha_base;
        ActuatorBase* _actuator_beta_base;
        ActuatorBase* _actuator_gamma_base;
        ActuatorBase* _actuator_delta_base;
        ActuatorBase* _actuator_epsilon_base;

    
    private:
    
};