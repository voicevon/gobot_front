#pragma once
#include "MyBoards/const.h"
#include "stdint.h"
#include "range_constraint/actuator_range_constraint_base.h"
#include "Mylibs/basic/logger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/planner/queue_move_block.h"
#include "CNC/planner/queue_line_segment.h"
#include "MyLibs/list/list_base.h"


class ActuatorBase: public ListItem{
    public:
        char MyName;
        // Don't remove these lines.  
        // enum EnumActuatorState{
        //     IDLE,
        //     FOLLOW_SPEED_ONLY,    // For AGV travelling.  or CNC spindle liked.
        //     FOLLOW_POSITION_SPEED,  // Currently, we are using
        //     FOLLOW_POSITION_ACCEL_SPEED,  //For CNC actuator
        // };
        // EnumActuatorState ActuatorState;

        virtual void SpinOnce();
        void LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint);
        virtual void UpdateMovement(MoveBlock_SingleActuator* movement);

        float GetCurrentPosition(){return this->_current_position;};

        bool IsMoving(){return this->__is_moving;};
        virtual void ForceStop();   //Only G28 is using this.

        virtual void InitFormular_FromCncPosition(float position_in_cnc_unit);
        virtual float ConvertPosition_ToCncUnit(float actuator_position){};
        float GetPosition_FromCncUnit(float cnc_position){};

    protected:


        float _target_position;   // is always an absolute position
        float _current_position;
        float _current_velocity;   // todo: only speed control need this, known:  dc_motor_actuator.
        bool __is_moving = false;

        ActuatorRangeConstraintBase* _range_constraint;
        bool _is_range_constraint = false;
        //TODO:   float _current_cnc_position;  
    private:
    
};