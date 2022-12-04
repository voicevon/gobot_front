#include "cnc_mover_base.h"
#include "teensy_step_gateway.h"

void CncMover::SpinOnce(){
    // Logger::Debug("CncMover::SpinOnce()");
    bool has_moving_actuator = false;
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        CncActuatorBase* actuator = CncActuator_List::Instance().GetActuator(a);
        actuator->SpinOnce();
        if (actuator->IsMoving()) has_moving_actuator= true;
    }

    if (Queue_MoveBlock::Instance().BufferIsEmpty()) {
        // Logger::Print("CncMover::SpinOnce() Queue_MoveBlock::  Buffer is Empty", 91);
        return;
    }
    // Logger::Print("CncMover::SpinOnce() Queue_MoveBlock::  Buffer got moveblock", 21);

    if (has_moving_actuator){
        delay(500);
        // Serial.print("M");
        return;
    }

    // Logger::Info("CncMover::SpinOnce() withdraw queue_move_block");
    MoveBlock* mb = Queue_MoveBlock::Instance().Withdraw();
    // Logger::Print("MoveBlocks[AXIS_ALPHA].TargetPosition", mb->MoveBlocks[AXIS_ALPHA].TargetPosition);
    // Logger::Print("MoveBlocks[AXIS_ALPHA].Speed", mb->MoveBlocks[AXIS_ALPHA].Speed);
    // Logger::Print("MoveBlocks[AXIS_ALPHA].Acceleration", mb->MoveBlocks[AXIS_ALPHA].Acceleration);

    this->AllActuatorsMoveTo(mb);
    // Logger::Print("CncMover::SpinOnce() point", 99);

}

void CncMover::AllActuatorsMoveTo(MoveBlock* move){
    CncActuatorBase* act;
    Logger::Debug("CncMover::AllActuatorsMoveTo()");
    move->PrintOut("From CncMover::AllActuatorsMoveTo()");
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        // TODO: transloate postion unit here ??  What actuator is using?
        // TODO: translate speed unit here.!!
        // Logger::Print("axis = ", CncAxis::From_index_toName());
        act =  CncActuator_List::Instance().GetActuator(a);
        act->UpdateMovement(&move->MoveBlocks[a]);
    }

    // Please keep this line before 2023 September.
    // TeensyStep_Gateway::Instance().AsyncMove();   // When use TeensyStep driver , this is necessary

}

void CncMover::AllActuatorsStop(){
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        CncActuator_List::Instance().GetActuator(a)->ForceStop();
    }
}


void CncMover::SetActuatorCurrentCncPositionAs(EnumAxis_Inverseinematic actuator_name, float as_current_position){
    CncActuator_List::Instance().GetActuator(actuator_name)->Formular_SetRawOffset(as_current_position);
}

float CncMover::GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis_Inverseinematic actuator_name){
    Logger::Debug("CncMover::GetSingleActuatorCurrentPosition_InCncUnit() ");
    return CncActuator_List::Instance().GetActuator(actuator_name)->GetCurrentPosition();
}


//TODO:  be a virtual method.
float CncMover::GetAbsDistanceToTarget_InCncUnit(){
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