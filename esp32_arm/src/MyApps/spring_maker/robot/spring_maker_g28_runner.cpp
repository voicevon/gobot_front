#include "spring_maker_g28_runner.h"


void SpringMaker_G28_Runner::Init( CncMoverBase* mover){
    Logger::Info("SpringMaker_G28_Runner::Init() Hoiming_config");
    this->__mover = mover;

    Logger::Info("SpringMaker_G28_Runner::Init() Alpha axis home_triggers");
    PositionTrigger* trigger;
    trigger = HomeTrigger_Array::Instance().GetPositionTrigger(0);
    trigger->AxisName = 'X';
    trigger->SetTriggerPosition(TWO_PI* 1 / 386);       

}


void SpringMaker_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("SpringMaker_G28_Runner::SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case AXIS_X:
            Logger::Print("SpringMaker_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("SpringMaker_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            Logger::Print("SpringMaker_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            move->IsAbsTargetPosition = false;
            move->TargetPosition = TWO_PI;
            move->Speed = 100.0f;
            move->Acceleration = 1.0f;
            break;
        default:
            Logger::Error(" SpringMaker_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("Jail Breaking");
            break;
    }
    Logger::Print("SpringMaker_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}
