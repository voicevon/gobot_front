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
    trigger->AxisName='a';
    trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
}

void TeethWarehouse_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* alpha;
    MoveBlock_SingleActuator* beta;
    mb->DeepReset_ToDefault();

    switch (axis){
        case 'X':
            Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            // Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            alpha = &mb->MoveBlocks[AXIS_ALPHA];
            // alpha->IsAbsTargetPosition = false;
            alpha->TargetPosition = 99999;
            alpha->Speed = 0.1;
            alpha->Acceleration = 0.05;
            beta = &mb->MoveBlocks[AXIS_BETA];
            // beta->IsAbsTargetPosition = false;
            beta->TargetPosition = -99999;
            beta->Speed = 0.1;
            beta->Acceleration = 0.05;
            break;

        case 'a':
            Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            // Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            alpha = &mb->MoveBlocks[AXIS_ALPHA];
            // alpha->IsAbsTargetPosition = false;
            alpha->TargetPosition = -99999;
            alpha->Speed = 0.1;
            alpha->Acceleration = 0.05;
            beta = &mb->MoveBlocks[AXIS_BETA];
            // beta->IsAbsTargetPosition = false;
            beta->TargetPosition = -99999;
            beta->Speed = 0.1;
            beta->Acceleration = 0.05;
            break;

        default:
            Logger::Error(" TeethWarehouse_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("TeethWarehouse_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}


