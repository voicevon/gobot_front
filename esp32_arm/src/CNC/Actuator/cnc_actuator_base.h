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
        enum EnumActuatorMachenicType{
            CONTINUROUS_ROTARY = 1,  // stepper, dc-motor, bldc
            LIMITED_ROTARY = 2,      // servo
            LIMITED_LINEAR = 3,      // 
        };
        EnumActuatorMachenicType MachenicType;
        virtual void SpinOnce();
        void LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint);
        virtual void UpdateMovement(MoveBlock_SingleActuator* movement);
        virtual void ForceStop();   //Only G28 is using this.
        virtual float GetCurrentPosition(){};   //TODO:  remove body
        virtual void SetCurrentPositionAs(float new_position){};    //TODO: remove body
        void Recalulate_distance_speed_acceleration(MoveBlock_SingleActuator* move, float start_position, float target_time);
        bool IsMoving(){return _is_moving;};   // TODO:  is this necessary?


        // TODO: remove below 3.
        void Formular_SetSlope(float slope_from_actuator_to_cnc){
            this->__formular_slope_from_raw_to_cnc=slope_from_actuator_to_cnc;
            };
        void Formular_SetRawOffset(float current_cnc_position);
        float ConvertPosition_ToCncUnit(float actuator_position);

    protected:
        float _limited_speed = 100;
        float _limited_accelleration = 100;
        // TODO:  remove below 2
        float GetPosition_FromCncUnit(float cnc_position);
        float GetSpeed_FromCncUnit(float cnc_speed);


        float _target_position;   // is always an absolute position
        float _current_position;  //TODO: remove this
        bool _is_moving = false;   // a: Not reach target_position,  b:Force stopped.

        ActuatorRangeConstraintBase* _range_constraint;
        bool _is_range_constraint = false;


    private:
        float __formular_slope_from_raw_to_cnc = 1.0;   //TODO:  remove this
        float __formular_raw_offset = 0;              //TODO: remove this ??
};