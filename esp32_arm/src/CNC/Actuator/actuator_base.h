#pragma once
#include "MyBoards/const.h"
class ActuatorBase{
    public:
        // virtual void Init();
        virtual float GetCurrentPosition_InCncUnit();
        virtual void SetPosition(float position_in_cnc_unit);
        virtual void MoveTo(bool is_absolute_position, float position_in_cnc_unit);
        virtual float GetDistanceToTarget_InCncUnit();
        virtual void SetSpeed(float speed_per_second);

    protected:
        float _target_cnc_position;   // is always an absolute position
    private:
    
};