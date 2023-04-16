#pragma once
#include "range_constraint/actuator_range_constraint_base.h"
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/planner/move_blocks_queue.h"
#include "CNC/planner/line_segments_queue.h"
#include "MyLibs/basic/list/list_base.h"
#include "Mylibs/basic/logger.h"
#include "stdint.h"



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
        virtual void ForceStop_G28_Only(bool update_target_position = true);   //Only G28 is using this.
        virtual float GetCurrentPosition();
        virtual void SetCurrentPositionAs(float new_position);
        void Recalulate_distance_speed_acceleration(MoveBlock_SingleActuator* move, float start_position, float target_time);
        bool IsMoving(){return _is_moving;};   // TODO:  is this necessary?

        // virtual void TurnToSleep_WhenIdle(){};   // Only stepper is avaliable. 2023 March.
        // void SetAutoSleep(int idle_seconds_to_sleep) {_idle_seconds_to_sleep = idle_seconds_to_sleep;};  //After idle_seconds, will diable actuator or driver. default is -1, saying disabled.

    protected:
        // virtual void SetSleep(bool enabled)
        float _target_position;   // is always an absolute position
        bool _is_moving = false;   // a: Not reach target_position,  b:Force stopped.
        // int _idle_seconds_to_sleep = -1;
    private:
};