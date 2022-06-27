
#pragma once

#include <stdint.h>
#include "../coordinate/cnc_axis.h"
// 
// TODO:  template <actuator>
class CncMoverBase{
    public:
        //TODO:   void LinkActuator(char actuator_name,  ActuatorBase* actuator );

        virtual void PrintOut(const char* title);
        virtual void SetActuatorSpeed(EnumAxis actuator_name, float speed_per_second);  // TODO:  remove this
        virtual void SetEefSpeed(float speed);
        // void SetBlockedMove(bool is_blocked_move){this->_is_blocked_move=is_blocked_move;};

        virtual void SetActuatorCurrentCncPositionAs(EnumAxis actuator_name, float as_current_position);

        virtual void AllActuatorsMoveTo(uint8_t is_absolute_position_flags, float* positions_in_cnc_unit);
        virtual void SingleActuatorMoveTo(EnumAxis actuator_name, bool is_absolute_position,  float position_in_cnc_unit);
        virtual void AllActuatorsStop();  // TODO: with flags
        virtual void SingleActuatorStop(EnumAxis actuator_name);

        virtual bool ActuatorIsMoving(EnumAxis actuator_name);
        virtual float GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis actuator_name);  // TODO: return  all current position.
        virtual float GetAbsDistanceToTarget_InCncUnit();
        void SetMovingFlags(uint8_t target_actuator_flags){this->_moving_actuator_flags=target_actuator_flags;};


    protected:
        // bool _is_blocked_move;
        uint8_t _moving_actuator_flags;
    
    private:
    
};