#include "mover_base.h"


// bool MotorIsMoving(char moto_name);
void MoverBase::SpinOnce(){
    // Logger::Debug("MoverBase::SpinOnce()");
    _actuator_alpha_base->SpinOnce();
    _actuator_beta_base->SpinOnce();
    _actuator_delta_base->SpinOnce();

    if (this->__queue_move_block->BufferIsEmpty()) return;

    MoveBlock* mb = this->__queue_move_block->FetchTailMoveBlock();
    this->AllActuatorsMoveTo(mb);
    // _actuator_alpha_base->UpdateMovement(&mb->MoveBlocks[AXIS_ALPHA]);
    // _actuator_beta_base->UpdateMovement(&mb->MoveBlocks[AXIS_BETA]);
    // _actuator_delta_base->UpdateMovement(&mb->MoveBlocks[AXIS_GAMMA]);
}

// void MoverBase::SetActuatorSpeed(EnumAxis actuator_name, float steps_per_second){
//     if (actuator_name ==AXIS_ALPHA){
//         this->_actuator_alpha_base->SetSpeed(steps_per_second);
//     }else if(actuator_name == AXIS_BETA){
//         this->_actuator_beta_base->SetSpeed(steps_per_second);
//     }else{
//         Logger::Halt("MoverBase::SetActuatorSpeed() ");
//     }
// }

// void MoverBase::SetActuatorAcceleration(EnumAxis axis, float accelleration){
//     if (axis ==AXIS_ALPHA){
//         this->_actuator_alpha_base->SetAccelleration(accelleration);
//     }else if(axis == AXIS_BETA){
//         this->_actuator_beta_base->SetAccelleration(accelleration);
//     }else{
//         Logger::Halt("Mover_DualStepper::SetActuatorSpeed() ");
//     }
// }

void MoverBase::SetActuatorCurrentCncPositionAs(EnumAxis actuator_name, float as_current_position){
    if (actuator_name == AXIS_ALPHA){
        this->_actuator_alpha_base->SetCurrentPositionAs(as_current_position);
    }else if (actuator_name == AXIS_BETA){
        this->_actuator_beta_base->SetCurrentPositionAs(as_current_position);
    }else{
        Logger::Warn("MoverBase::SingleMotorMoveTo()");
        Serial.print("Unkonwn axisname= ");
        Serial.print(actuator_name);
        Serial.println(FCBC_RESET);
    }
}

float MoverBase::GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis actuator_name){
    Logger::Debug("MoverBase::GetSingleActuatorCurrentPosition_InCncUnit() ");
    if (actuator_name == AXIS_ALPHA){
        Logger::Print("MoverBase::GetSingleActuatorCurrentPosition_InCncUnit() point", 1);
        auto aa = this->_actuator_alpha_base;
        Logger::Print("MoverBase::GetSingleActuatorCurrentPosition_InCncUnit() point", 2);
        auto bb = aa->GetCurrentPosition();
        Logger::Print("MoverBase::GetSingleActuatorCurrentPosition_InCncUnit() bb", bb);

        return this->_actuator_alpha_base->GetCurrentPosition();


    }else if (actuator_name == AXIS_BETA){
        return this->_actuator_beta_base->GetCurrentPosition();

    }else{
        log_w("Mover_DualStepper::SingleMotorMoveTo() axisname= ", actuator_name );
    }
    return 0;
}

float MoverBase::GetAbsDistanceToTarget_InCncUnit(){
    float alpha_distance = 0;
    if((this->_moving_actuator_flags & 0x01) > 0){
       alpha_distance = this->_actuator_alpha_base->GetAbsDistanceToTarget_InCncUnit();
    }
    float beta_distance = 0;
    if ((this->_moving_actuator_flags & 0x02) > 0){
       beta_distance = this->_actuator_beta_base->GetAbsDistanceToTarget_InCncUnit();
    }
    bool debug= false;
    if(debug){
        Serial.print("[Debug] Mover_DualStepper::GetAbsDistanceToTarget_InCncUnit() alpha = ");
        Serial.print(alpha_distance);
        Serial.print("  beta = ");
        Serial.println(beta_distance);
    }
    return sqrt(alpha_distance * alpha_distance + beta_distance * beta_distance);
}