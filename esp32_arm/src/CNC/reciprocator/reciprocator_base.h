#pragma once
#include "CNC/Actuator/cnc_actuator_base.h"
#include "Robot/axis_homer/position_trigger.h"



class ReciprocatorBase{
    public:
        enum EnumState{
            IDLE = 1,
            MOVING_TO_TRIGGER_POSITION = 21,
            MOVING_TO_ENCODER_POSITION = 22,
            READY = 31,
            HOLDING = 32,

        };

        void LinkActuator(CncActuatorBase* actuator){_actuator = actuator;};
        void LinkPositionTriger(PositionTrigger* trigger_at_min_position, PositionTrigger* trigger_at_max_position){
            __trigger_at_min_position = trigger_at_min_position;
            __trigger_at_max_position = trigger_at_max_position;
        };
        virtual void MoveToTriggerPosition(PositionTrigger* target_position_trigger){};
        virtual void MoveToEncoderPosition(uint32_t encoder_position){};
        void SpinStateMachine();

    protected:
        CncActuatorBase* _actuator;
        EnumState _state;

    private:
        PositionTrigger* __trigger_at_min_position;
        PositionTrigger* __trigger_at_max_position;


};

