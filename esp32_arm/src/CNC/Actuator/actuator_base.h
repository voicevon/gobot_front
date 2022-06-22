#pragma once
#include "MyBoards/const.h"
#include "stdint.h"

class ActuatorBase{
    public:
        // virtual void Init();
        
        void InitCncRange(float min, float max){this->_cnc_range_min=min;this->_cnc_range_max=max;};
        virtual float GetCurrentPosition_InCncUnit();
        virtual void SetCurrentPositionAs(float position_in_cnc_unit);
        virtual void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit);
        virtual float GetDistanceToTarget_InCncUnit();
        virtual void SetSpeed(float speed_per_second);
        virtual void SetAccelleration(float accelleration_per_sqr_second){};  // TODO: remove defination.
        virtual void Stop();   //TODO:  Remove from base.

    protected:
        void _SetActuatorRange(float min, float max){this->_actuator_range_min=min;this->_actuator_range_max=max;};
        virtual float _ConvertTo_ActuatorRange(float cnc_position){return 0;};
        virtual float _ConvertTo_CncRange(float actuator_position){return 0;};

        float _cnc_range_min;
        float _cnc_range_max;
        float _actuator_range_min = -99999;
        float _actuator_range_max = 99999;
        float _target_cnc_position;   // is always an absolute position
        //TODO:   float _current_cnc_position;  
    private:
    
};