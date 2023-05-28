#pragma once
#include "CNC/Actuator/cnc_actuator_base.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger.h"


class Reciprocator{
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
        void MoveToTriggerPosition(bool go_to_positive_dir);
        void MoveToEncoderPosition(uint32_t encoder_position);
        void SpinOnce_Statemahcine();
        EnumState GetState(){return _state;};

        // PositionTrigger* GetTrigger_for_MinPosition(){return __trigger_at_min_position;};
        // PositionTrigger* GetTrigger_for_MaxPostion(){return __trigger_at_max_position;};
        

    protected:
        CncActuatorBase* _actuator;
        EnumState _state;
        // virtual void _MoveToTriggerPosition();


    private:
        PositionTrigger* __trigger_at_min_position;
        PositionTrigger* __trigger_at_max_position;
        PositionTrigger* __towarding_position_trigger;


};

