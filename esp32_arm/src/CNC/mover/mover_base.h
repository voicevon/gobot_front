
#pragma once

#include <stdint.h>
#include "../coordinate/cnc_axis.h"
#include "MyLibs/basic/logger.h"
#include "CNC/board/cnc_board_base.h"
#include "../Actuator/actuator_base.h"




// TODO:  template <actuator>
class MoverBase{
    public:
        // virtual void SingleActuatorMoveTo(LineSegment* movement);
        virtual void SingleActuatorMoveTo(MoveBlock* move_block);

        //TODO:   void LinkActuator(char actuator_name,  ActuatorBase* actuator );
        virtual void PrintOut(const char* title);
        void SetActuatorSpeed(EnumAxis axis, float speed);  
        void SetActuatorAcceleration(EnumAxis axis, float accelleration);
        void SetActuatorCurrentCncPositionAs(EnumAxis actuator_name, float as_current_position);
        float GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis actuator_name);  // TODO: return  all current position.

        virtual void SetEefSpeed(float speed);

        void SetMovingFlags(uint8_t target_actuator_flags){this->_moving_actuator_flags=target_actuator_flags;};

        virtual void AllActuatorsMoveTo(uint8_t is_absolute_position_flags, float* positions_in_cnc_unit);
        virtual void AllActuatorsStop();  // TODO: with flags
        virtual void SingleActuatorStop(EnumAxis actuator_name);

        virtual bool ActuatorIsMoving(EnumAxis actuator_name);
        virtual bool HasArrivedTargetPosition();
        float GetAbsDistanceToTarget_InCncUnit();

        Queue_MoveBlock* __queue_move_block;

    protected:
        // bool _is_blocked_move;
        uint8_t _moving_actuator_flags;
        float _small_distance_for_arriving_target_position = 0.1;
        
        ActuatorBase* _actuator_alpha_base;
        ActuatorBase* _actuator_beta_base;
        ActuatorBase* _actuator_gamma_base;
        ActuatorBase* _actuator_delta_base;
        ActuatorBase* _actuator_epsilon_base;

    
    private:
    
};