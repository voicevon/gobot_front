#include "reciprocator.h"



void Reciprocator::SpinOnce_Statemahcine(){
    _actuator->SpinOnce();
    if (_state == EnumState::IDLE){

    }
    if (_state == EnumState::MOVING_TO_TRIGGER_POSITION){
        if (__trigger_at_max_position->IsFired()){
            _actuator->ForceStop_G28_Only();
            _state = EnumState::READY;
        }
    }
    if (_state == EnumState::MOVING_TO_ENCODER_POSITION){
        Logger::Warn("Reciprocator::SpinOnce_Statemahcine() MOVING_TO_ENCODER_POSITION  is in TODO list");
    }
    if (_state == EnumState::READY){

    }
    if (_state == EnumState::HOLDING){

    }
    
}

void Reciprocator::MoveToTriggerPosition(bool go_to_positive_dir){
    MoveBlock_SingleActuator move;
    move.Speed = 100;
    if (go_to_positive_dir){
        __towarding_position_trigger = __trigger_at_max_position;
        move.TargetPosition = 999999;
        _actuator->UpdateMovement(&move);
    }else{
        __towarding_position_trigger = __trigger_at_min_position;
        move.TargetPosition = -999999;
        _actuator->UpdateMovement(&move);
    }
}

void Reciprocator::MoveToEncoderPosition(uint32_t encoder_position){
    
}