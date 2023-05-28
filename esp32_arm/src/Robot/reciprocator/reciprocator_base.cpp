#include "reciprocator_base.h"




void ReciprocatorBase::SpinOnce_Statemahcine(){
    if (_state == EnumState::IDLE){

    }
    if (_state == EnumState::MOVING_TO_TRIGGER_POSITION){
        if (__trigger_at_max_position->IsFired()){
            _actuator->ForceStop_G28_Only();
            _state = EnumState::READY;
        }
    }
    if (_state == EnumState::MOVING_TO_ENCODER_POSITION){

    }
    if (_state == EnumState::READY){

    }
    if (_state == EnumState::HOLDING){

    }
    
}

void ReciprocatorBase::MoveToTriggerPosition(PositionTrigger* target_position_trigger){
    _towarding_position_trigger = target_position_trigger;
    _MoveToTriggerPosition();
}

void ReciprocatorBase::MoveToEncoderPosition(uint32_t encoder_position){
    
}