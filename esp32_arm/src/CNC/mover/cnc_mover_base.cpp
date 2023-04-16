#include "cnc_mover_base.h"

void CncMover::SpinOnce(){
    // Logger::Debug("CncMover::SpinOnce()");
    static uint32_t last_print_time_stamp = 0;

    bool has_moving_actuator = false;
    for(int a=0; a<gs_CncActuator_List::Instance().GetItemsCount(); a++){
        CncActuatorBase* actuator = gs_CncActuator_List::Instance().GetActuator(a);
        actuator->SpinOnce();
        if (actuator->IsMoving()) {
            has_moving_actuator= true;
        }
    }
    // gs_CncActuator_List::Instance().PrintOUt("caller is CncMover::SpinOnce()");


    // Logger::Print("CncMover::SpinOnce() gs_MoveBlock_Queue::  Buffer got moveblock 21", 21);

    if (has_moving_actuator){
        if (millis() - last_print_time_stamp > 500){
            Serial.print("m");
            last_print_time_stamp = millis();
        }
        return;
    }
    
    if (gs_MoveBlock_Queue::Instance().BufferIsEmpty()) {
        // Logger::Print("CncMover::SpinOnce() gs_MoveBlock_Queue::  Buffer is Empty 91", 91);
        return;
    }

    // Logger::Print("CncMover::SpinOnce() withdraw queue_move_block  31", 31);
    // gs_MoveBlock_Queue::Instance().PrintOut("caller is CncMover::SpinOnce()");
    MoveBlock* mb = gs_MoveBlock_Queue::Instance().WithdrawTailElement();
    // Logger::Print("MoveBlocks[AXIS_ALPHA].TargetPosition", mb->MoveBlocks[AXIS_ALPHA].TargetPosition);
    // Logger::Print("MoveBlocks[AXIS_ALPHA].Speed", mb->MoveBlocks[AXIS_ALPHA].Speed);
    // Logger::Print("MoveBlocks[AXIS_ALPHA].Acceleration", mb->MoveBlocks[AXIS_ALPHA].Acceleration);

    this->AllActuatorsMoveTo(mb);
    // mb->PrintOut("caller is  CncMover::SpinOnce()");
    Logger::Print("CncMover::SpinOnce() point   99", 99);

}

void CncMover::AllActuatorsMoveTo(MoveBlock* move){
    CncActuatorBase* act;
    // Logger::Debug("CncMover::AllActuatorsMoveTo()");
    // move->PrintOut("From CncMover::AllActuatorsMoveTo()");
    for(int a=0; a<gs_CncActuator_List::Instance().GetItemsCount(); a++){
        // TODO: transloate postion unit here ??  What actuator is using?
        // TODO: translate speed unit here.!!
        // Logger::Print("axis = ", CncAxis::From_index_toName());
        act =  gs_CncActuator_List::Instance().GetActuator(a);
        act->UpdateMovement(&move->MoveBlocks[a]);
    }

    // Please keep this line before 2023 September.
    // TeensyStep_Gateway::Instance().AsyncMove();   // When use TeensyStep driver , this is necessary

}

void CncMover::AllActuator_SetPositionAs(IKPosition_abgdekl* new_position){
    CncActuatorBase* act;
    for(int a=0; a<gs_CncActuator_List::Instance().GetItemsCount(); a++){
        // Logger::Print("axis = ", CncAxis::From_index_toName());
        act =  gs_CncActuator_List::Instance().GetActuator(a);
        act->SetCurrentPositionAs(new_position->Positions[a]);
    }
}

void CncMover::AllActuatorsStop(){
    for(int a=0; a<gs_CncActuator_List::Instance().GetItemsCount(); a++){
        gs_CncActuator_List::Instance().GetActuator(a)->ForceStop_G28_Only();
    }
}



bool CncMover::IsMoving(){
    for(int a=0; a<gs_CncActuator_List::Instance().GetItemsCount(); a++){
        if (gs_CncActuator_List::Instance().GetActuator(a)->IsMoving() )
            return true;
    }
    return false;
}

