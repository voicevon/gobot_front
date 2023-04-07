#include "cnc_mover_base.h"

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
    // Logger::Debug("CncMover::AllActuatorsMoveTo()");
    // move->PrintOut("From CncMover::AllActuatorsMoveTo()");
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

void CncMover::AllActuator_SetPositionAs(IKPosition_abgdekl* new_position){
    CncActuatorBase* act;
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        // Logger::Print("axis = ", CncAxis::From_index_toName());
        act =  CncActuator_List::Instance().GetActuator(a);
        act->SetCurrentPositionAs(new_position->Positions[a]);
    }
}

void CncMover::AllActuatorsStop(){
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        CncActuator_List::Instance().GetActuator(a)->ForceStop();
    }
}



bool CncMover::IsMoving(){
    for(int a=0; a<CncActuator_List::Instance().GetItemsCount(); a++){
        if (CncActuator_List::Instance().GetActuator(a)->IsMoving() )
            return true;
    }
    return false;
}

