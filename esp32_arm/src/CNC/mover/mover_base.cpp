#include "mover_base.h"


void MoverBase::SpinOnce(){
    // Logger::Debug("MoverBase::SpinOnce()");
    _actuator_alpha_base->SpinOnce();
    // _actuator_beta_base->SpinOnce();
    // _actuator_delta_base->SpinOnce();

    // Logger::Print("MoverBase::SpinOnce() point", 1);
    if (Queue_MoveBlock::Instance().BufferIsEmpty()) {
        // Logger::Print("MoverBase::SpinOnce() point", 91);
        return;
    }
    Logger::Print("MoverBase::SpinOnce() point", 2);
    
    Logger::Info("MoverBase::SpinOnce() fetching queue_moveb_lock");
    MoveBlock* mb = Queue_MoveBlock::Instance().FetchTailMoveBlock();
    Logger::Print("MoveBlocks[AXIS_ALPHA].TargetPosition", mb->MoveBlocks[AXIS_ALPHA].TargetPosition);
    this->AllActuatorsMoveTo(mb);
    Logger::Print("MoverBase::SpinOnce() point", 99);

}

void MoverBase::LinkActuator(char actuator_name, ActuatorBase* actuator){
    switch (actuator_name){
    case 'A':
        this->_actuator_alpha_base = actuator;
        break;
    case 'B':
        this->_actuator_beta_base = actuator;
        break;
    case 'C':
        this->_actuator_gamma_base = actuator;
        break;
    
    default:
        Logger::Warn("MoverBase::LinkActuator()");
        Logger::Print("actuator_name", actuator_name);
        break;
    }
}

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