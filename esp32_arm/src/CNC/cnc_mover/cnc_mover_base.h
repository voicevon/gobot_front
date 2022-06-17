
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

    protected:
    
    private:
    
};