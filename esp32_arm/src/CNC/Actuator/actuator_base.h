#pragma once
#include "MyBoards/const.h"
#include "stdint.h"
#include "range_constraint/actuator_range_constraint_base.h"
#include "Mylibs/basic/logger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/gcode/line_segment_queue.h"   //TODO:  use block



class ActuatorBase{
    public:
        void LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint);
        virtual float GetCurrentPosition(){return this->_current_cnc_position;};
        virtual void SetCurrentPositionAs(float position_in_cnc_unit);
        // virtual void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit);
        virtual void UpdateMovement(LineSegment* movement);
        virtual float GetAbsDistanceToTarget_InCncUnit();

        float GetTartetCncPosition(){return this->_target_cnc_position;};
        float GetCurrentCncPosition(){return this->_current_cnc_position;};

        //Speed and acceleration
        virtual void ForceStop();   //Only G28 is using this.
        virtual void UpdateTargetPositionFromCurrent();
        virtual float GetSpeed();    // ?? TODO:  var should be in base class ?
        virtual void SetSpeed(float speed_in_cnc_unit);   // TODO:  remove, merge to LineSegment.
        float GetNeededSeconds();
        void RenewSpeed(float moving_time);
        virtual void SetAccelleration(float accelleration_in_cnc_unit);  // TODO: remove defination.

    protected:
        float _target_cnc_position;   // is always an absolute position
        float _current_cnc_position;

        ActuatorRangeConstraintBase* _range_constraint;
        bool _is_range_constraint = false;
        //TODO:   float _current_cnc_position;  
    private:
    
};