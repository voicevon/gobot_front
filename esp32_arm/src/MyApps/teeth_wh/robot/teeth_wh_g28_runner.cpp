#include "teeth_wh_g28_runner.h"


void TeethWarehouse_G28_Runner::Init(CncMoverBase* mover){
    Logger::Info("TeethWarehouse_G28_Runner::Init() Hoiming_config");
    this->__mover = mover;

    Logger::Info("TeethWarehouse_G28_Runner::Init() Alpha axis home_triggers");
    PositionTrigger* trigger;
    trigger = HomeTrigger_Array::Instance().GetPositionTrigger(0);
    trigger->AxisName = 'X';
    trigger->SetTriggerPosition(TWO_PI* 1 / 386);      // @01 pitch, total 386 pitches,    

    trigger = HomeTrigger_Array::Instance().GetPositionTrigger(1);
    trigger->AxisName='X';
    trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
}

void TeethWarehouse_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case 'X':
            // Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            // Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            // Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            move->IsAbsTargetPosition = false;
            move->TargetPosition = TWO_PI;
            move->Speed = 0.1;
            move->Acceleration = 0.05;
            break;
        default:
            Logger::Error(" TeethWarehouse_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}


