#include "vsc_g28_runner.h"


void Vsc_G28_Runner::Init(Vsc_Board* board, MoverBase* mover){
    Logger::Info("Vsc_G28_Runner::Init() Hoiming_config");
    this->__mover = mover;

    // homing.IsDirectionToMax = true;
    // homing.Speed = TWO_PI / 60;  // 60 seconds Per loop.
    // homing.Accelleration = 0.1;
    // homing.DistanceToGo = TWO_PI;
    
    Logger::Info("Vsc_G28_Runner::Init() Alpha axis home_triggers");
    PositionTrigger* trigger;
    trigger = board->GetPositionTrigger(0);
    trigger->AxisName='X';
    trigger->SetTriggerPosition(TWO_PI* 1 / 386);      // @01 pitch, total 386 pitches,    

    trigger = board->GetPositionTrigger(1);
    trigger->AxisName='X';
    trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
}


void Vsc_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("Vsc_G28_Runner::SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    int axis_index = 0;
    switch (axis){
        case 'X':
            axis_index = 0;
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[axis_index];
            Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            move->IsAbsTargetPosition = false;
            move->TargetPosition = TWO_PI;
            move->Speed = 0.1;
            move->Acceleration = 0.05;
            break;
        default:
            Logger::Error(" Vsc_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}


