#pragma once
#include "MyBoards/const.h"
#include "stdint.h"
#include "range_constraint/actuator_range_constraint_base.h"
#include "Mylibs/basic/logger.h"
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/planner/queue_move_block.h"
#include "CNC/planner/queue_line_segment.h"
#include "MyLibs/list/list_base.h"



class CncActuatorBase: public ListItem{
    public:
        char MyName;
        // Don't remove this enum.  
        // enum EnumActuatorState{
        //     IDLE,
        //     FOLLOW_SPEED_ONLY,    // For AGV travelling.  or CNC spindle liked.
        //     FOLLOW_POSITION_SPEED,  // Currently, we are using
        //     FOLLOW_POSITION_ACCEL_SPEED,  //For CNC actuator
        // };
        // EnumActuatorState ActuatorState;
        // enum EnumActuatorMachenicType{
        //     CONTINUROUS_ROTARY = 1,  // stepper, dc-motor, bldc
        //     LIMITED_ROTARY = 2,      // servo
        //     LIMITED_LINEAR = 3,      // 
        // };
        // EnumActuatorMachenicType MachenicType;
        virtual void SpinOnce();
        virtual void UpdateMovement(MoveBlock_SingleActuator* movement);
        virtual void ForceStop();   //Only G28 is using this.
        virtual float GetCurrentPosition();
        virtual void SetCurrentPositionAs(float new_position);
        void Recalulate_distance_speed_acceleration(MoveBlock_SingleActuator* move, float start_position, float target_time);
        bool IsMoving(){return _is_moving;};   // TODO:  is this necessary?

    protected:
        float _target_position;   // is always an absolute position
        bool _is_moving = false;   // a: Not reach target_position,  b:Force stopped.

    private:
};