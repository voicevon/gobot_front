#include "box_mover_g28_runner.h"


void BoxMover_G28_Runner::Init(CncMover* mover){
    Logger::Info("BoxMover_G28_Runner::Init() Hoiming_config");
    this->__mover = mover;

    // homing.IsDirectionToMax = true;
    // homing.Speed = TWO_PI / 60;  // 60 seconds Per loop.
    // homing.Accelleration = 0.1;
    // homing.DistanceToGo = TWO_PI;
    
    Logger::Info("BoxMover_G28_Runner::Init() Alpha axis home_triggers");
    PositionTrigger* trigger;
    // trigger = board->GetPositionTrigger(0);
    trigger = HomeTrigger_Array::Instance().GetPositionTrigger(0);
    trigger->SetTriggerPosition(TWO_PI* 1 / 386);      // @01 pitch, total 386 pitches,
    trigger->AxisName = 'X' ;   

    // trigger = board->GetPositionTrigger(1);
    trigger = HomeTrigger_Array::Instance().GetPositionTrigger(1);
    trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
    trigger->AxisName = 'Z';
}



void BoxMover_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("BoxMover_G28_Runner::SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case AXIS_X:
            Logger::Print("BoxMover_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("BoxMover_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            Logger::Print("BoxMover_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            // move->IsAbsTargetPosition = false;
            move->TargetPosition = 123.1f;
            move->Speed = 0.2f;
            move->Acceleration = 0.1f;
            break;
        default:
            Logger::Error(" BoxMover_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("BoxMover_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}

// AxisHomer* BoxMover_G28_Runner::GetHomer(EnumAxis_ForwardKinematic axis){
//     return &__homer;
// }

// AxisHomer* BoxMover_G28_Runner::GetHomer(EnumAxis_Inverseinematic axis){
//     return &__homer;
// }
