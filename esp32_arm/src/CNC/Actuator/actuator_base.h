#pragma once
#include "MyBoards/const.h"
#include "stdint.h"
#include "range_constraint/actuator_range_constraint_base.h"

class ActuatorBase{
    public:
        void LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint);
        virtual float GetCurrentPosition_InCncUnit();
        virtual void SetCurrentPositionAs(float position_in_cnc_unit);
        virtual void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit);
        virtual float GetDistanceToTarget_InCncUnit();
        virtual void SetSpeed(float speed_per_second);
        virtual void SetAccelleration(float accelleration_per_sqr_second){};  // TODO: remove defination.
        virtual void Stop();   //TODO:  Remove from base.

    protected:
        // virtual float _ConvertTo_ActuatorRange(float cnc_position){return 0;};
        // virtual float _ConvertTo_CncRange(float actuator_position){return 0;};

        float _target_cnc_position;   // is always an absolute position
        ActuatorRangeConstraintBase* _range_constraint;
        bool _is_range_constraint = false;
        //TODO:   float _current_cnc_position;  
    private:
    
};