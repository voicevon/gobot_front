
#pragma once

// #include "CNC/Actuator/ActuatorBase.h"

// template <actuator>
class CncMoverBase{
    public:
        // void LinkActuator(char actuator_name,actuator );
        virtual void AllMotorsMoveTo(bool is_absolute_position, float* positions_in_cnc_unit, int motors_count);
        virtual void AllMotorStop();
        virtual void SingleMotorMoveTo(bool is_absolute_position, char motor_name, float position_in_cnc_unit);
        virtual float GetMotorPosition_InCncUnit(char motor_name);
        virtual void SetMotorPosition(char motor_name, float as_current_position);
        virtual float GetDistanceToTarget_InCncUnit();
        void SetBlockedMove(bool is_blocked_move){this->_is_blocked_move=is_blocked_move;};
        virtual void SetActuatorSpeed(char actuator_name, float speed_per_second);
        virtual bool MotorIsMoving(char motor_name);
        // virtual void Stop(char motor_name);
        virtual void SingleMotorStop(char motor_name);


    protected:
        bool _is_blocked_move;
    
    private:
    
};