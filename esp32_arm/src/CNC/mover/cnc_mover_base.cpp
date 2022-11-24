#include "cnc_mover_base.h"
#include "teensy_step_gateway.h"

void CncMoverBase::SpinOnce(){
    // Logger::Debug("CncMoverBase::SpinOnce()");
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        // Logger::Print("axis=", a);
        // Logger::Print("axis name=", CncActuator_List::Instance().GetActuator(a)->MyName);
        CncActuator_List::Instance().GetActuator(a)->SpinOnce();

    }

    // Logger::Print("CncMoverBase::SpinOnce() point", 1);
    if (Queue_MoveBlock::Instance().BufferIsEmpty()) {
        // Logger::Print("CncMoverBase::SpinOnce() Queue_MoveBlock::  Buffer is Empty", 91);
        return;
    }
    
    Logger::Info("CncMoverBase::SpinOnce() withdraw queue_move_block");
    MoveBlock* mb = Queue_MoveBlock::Instance().Withdraw();
    Logger::Print("MoveBlocks[AXIS_ALPHA].TargetPosition", mb->MoveBlocks[AXIS_ALPHA].TargetPosition);
    Logger::Print("MoveBlocks[AXIS_ALPHA].Speed", mb->MoveBlocks[AXIS_ALPHA].Speed);
    Logger::Print("MoveBlocks[AXIS_ALPHA].Acceleration", mb->MoveBlocks[AXIS_ALPHA].Acceleration);

    this->AllActuatorsMoveTo(mb);
    Logger::Print("CncMoverBase::SpinOnce() point", 99);

}

void CncMoverBase::AllActuatorsMoveTo(MoveBlock* move){
    CncActuatorBase* act;
    MoveBlock_SingleActuator * ms;
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        ms = &move->MoveBlocks[a];
        // TODO: transloate postion unit here ??  What actuator is using?
        // TODO: translate speed unit here.!!
        act =  CncActuator_List::Instance().GetActuator(a);
        act->UpdateMovement(ms);
    }

    // Please keep this line before 2023 September.
    // TeensyStep_Gateway::Instance().AsyncMove();   // When use TeensyStep driver , this is necessary

}

void CncMoverBase::AllActuatorsStop(){
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        CncActuator_List::Instance().GetActuator(a)->ForceStop();
    }
}


void CncMoverBase::SetActuatorCurrentCncPositionAs(EnumAxis_Inverseinematic actuator_name, float as_current_position){
    CncActuator_List::Instance().GetActuator(actuator_name)->ReInit_FormularOffset(as_current_position);
}

float CncMoverBase::GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis_Inverseinematic actuator_name){
    Logger::Debug("CncMoverBase::GetSingleActuatorCurrentPosition_InCncUnit() ");
    return CncActuator_List::Instance().GetActuator(actuator_name)->GetCurrentPosition();
}


//TODO:  be a virtual method.
float CncMoverBase::GetAbsDistanceToTarget_InCncUnit(){
    return 0.0f;
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