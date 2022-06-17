#pragma once
#include "MyBoards/const.h"
class ActuatorBase{
    public:
        // virtual void Init();
        virtual float GetCurrentPosition_InCncUnit();
        virtual void SetPosition(float position);
        virtual void MoveTo(bool is_absolute_position, float position);
        virtual float GetDistanceToTarget();
    protected:
        float _target_abs_position;
    private:
    
};