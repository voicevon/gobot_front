#include "smoker_g28_runner.h"


void Smoker_G28_Runner::Init(Smoker_Board* board, MoverBase* mover){
    Logger::Info("Smoker_G28_Runner::Init() Hoiming_config");
    this->__mover = mover;

    // homing.IsDirectionToMax = true;
    // homing.Speed = TWO_PI / 60;  // 60 seconds Per loop.
    // homing.Accelleration = 0.1;
    // homing.DistanceToGo = TWO_PI;
    
    Logger::Info("Smoker_G28_Runner::Init() Alpha axis home_triggers");
    PositionTrigger* trigger;
    trigger = board->GetPositionTrigger(0);
    trigger->SetTriggerPosition(TWO_PI* 1 / 386);      // @01 pitch, total 386 pitches,    
    // __homer.AppendPositionTrigger(trigger);

    trigger = board->GetPositionTrigger(1);
    trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
    // __homer.AppendPositionTrigger(trigger);
}



void Smoker_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("Smoker_G28_Runner::SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case AXIS_X:
            Logger::Print("Smoker_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("Smoker_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            Logger::Print("Smoker_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            move->IsAbsTargetPosition = false;
            // move->TargetPosition = homing.DistanceToGo;
            // move->Speed = homing.Speed;
            // move->Acceleration = homing.Accelleration;
            break;
        default:
            Logger::Error(" Smoker_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("Smoker_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}
