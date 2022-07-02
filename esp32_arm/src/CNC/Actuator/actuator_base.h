#pragma once
#include "MyBoards/const.h"
#include "stdint.h"
#include "range_constraint/actuator_range_constraint_base.h"
#include "Mylibs/basic/logger.h"

class ActuatorBase{
    public:
        void LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint);
        virtual float GetCurrentPosition_InCncUnit(){return this->_current_cnc_position;};
        virtual void SetCurrentPositionAs(float position_in_cnc_unit);
        virtual void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit);
        virtual float GetAbsDistanceToTarget_InCncUnit();
        virtual void SetSpeed(float speed_per_second);
        virtual void SetAccelleration(float accelleration_per_sqr_second){};  // TODO: remove defination.
        virtual float GetSpeed();    // ?? TODO:  var should be in base class ?
        float GetNeededSeconds();
        void RenewSpeed(float moving_time);
        virtual void Stop();   //TODO:  Remove from base.
        float GetTartetCncPosition(){return this->_target_cnc_position;};
        float GetCurrentCncPosition(){return this->_current_cnc_position;};

    protected:
        float _target_cnc_position;   // is always an absolute position
        float _current_cnc_position;

        ActuatorRangeConstraintBase* _range_constraint;
        bool _is_range_constraint = false;
        //TODO:   float _current_cnc_position;  
    private:
    
};