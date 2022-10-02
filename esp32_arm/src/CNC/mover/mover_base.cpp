#include "mover_base.h"


void MoverBase::SpinOnce(){
    Logger::Debug("MoverBase::SpinOnce()");
    for(int a=0; a<Actuator_List::Instance().GetItemsCount(); a++){
        Logger::Print("axis=", a);
        auto aa=Actuator_List::Instance();
        Logger::Print("MoverBase::SpinOnce() point", 31);
        auto bb=aa.GetActuator(a);
        Logger::Print("MoverBase::SpinOnce() point", 33);
        Serial.print(char(bb->MyName));
        Logger::Print("\t\tbb->MyName", bb->MyName);
        Actuator_List::Instance().GetActuator(a)->SpinOnce();
        Logger::Print("axis= spinoce over", a);

    }

    Logger::Print("MoverBase::SpinOnce() point", 1);
    if (Queue_MoveBlock::Instance().BufferIsEmpty()) {
        // Logger::Print("MoverBase::SpinOnce() Queue_MoveBlock::  Buffer is Empty", 91);
        return;
    }
    Logger::Print("MoverBase::SpinOnce() point", 2);
    
    Logger::Info("MoverBase::SpinOnce() withdraw queue_move_block");
    MoveBlock* mb = Queue_MoveBlock::Instance().Withdraw();
    Logger::Print("MoveBlocks[AXIS_ALPHA].TargetPosition", mb->MoveBlocks[AXIS_ALPHA].TargetPosition);
    Logger::Print("MoveBlocks[AXIS_ALPHA].Speed", mb->MoveBlocks[AXIS_ALPHA].Speed);
    Logger::Print("MoveBlocks[AXIS_ALPHA].Acceleration", mb->MoveBlocks[AXIS_ALPHA].Acceleration);

    this->AllActuatorsMoveTo(mb);
    Logger::Print("MoverBase::SpinOnce() point", 99);

}

void MoverBase::AllActuatorsMoveTo(MoveBlock* move){
    ActuatorBase* act;
    MoveBlock_SingleActuator * ms;
    for(int a=0; a<Actuator_List::Instance().GetItemsCount(); a++){
        ms = &move->MoveBlocks[a];
        act =  Actuator_List::Instance().GetActuator(a);

        if (ms->IsAbsTargetPosition){
            act->UpdateMovement(ms);
        }else if(ms->TargetPosition != 0){
            act->UpdateMovement(ms);
        }
    }
}

void MoverBase::AllActuatorsStop(){
    for(int a=0; a<Actuator_List::Instance().GetItemsCount(); a++){
        Actuator_List::Instance().GetActuator(a)->ForceStop();
    }
}


void MoverBase::SetActuatorCurrentCncPositionAs(EnumAxis_Inverseinematic actuator_name, float as_current_position){
    Actuator_List::Instance().GetActuator(actuator_name)->SetCurrentPositionAs(as_current_position);
}

float MoverBase::GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis_Inverseinematic actuator_name){
    Logger::Debug("MoverBase::GetSingleActuatorCurrentPosition_InCncUnit() ");
    return Actuator_List::Instance().GetActuator(actuator_name)->GetCurrentPosition();
}


//TODO:  be a virtual method.
float MoverBase::GetAbsDistanceToTarget_InCncUnit(){

    // float alpha_distance = 0;
    // if((this->_moving_actuator_flags & 0x01) > 0){
    //    alpha_distance = this->_actuator_alpha_base->GetAbsDistanceToTarget_InCncUnit();
    // }
    // float beta_distance = 0;
    // if ((this->_moving_actuator_flags & 0x02) > 0){
    //    beta_distance = this->_actuator_beta_base->GetAbsDistanceToTarget_InCncUnit();
    // }
    // bool debug= false;
    // if(debug){
    //     Serial.print("[Debug] Mover_DualStepper::GetAbsDistanceToTarget_InCncUnit() alpha = ");
    //     Serial.print(alpha_distance);
    //     Serial.print("  beta = ");
    //     Serial.println(beta_distance);
    // }
    // return sqrt(alpha_distance * alpha_distance + beta_distance * beta_distance);
}