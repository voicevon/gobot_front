#include "Twh_G28_Runner.h"
#include "circle_loop_arm_solution.h"


void Twh2Row_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution){
    Logger::Info("Twh2Row_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;
}

void Twh2Row_G28_Runner::SetMoveBlock_ToHome(char axis_name, MoveBlock* mb){
    Logger::Debug("Twh2Row_G28_Runner::SetMoveBlock_ToHome()" );
    Serial.print(char(axis_name));
    Logger::Print("\taxis", char(axis_name));
    MoveBlock_SingleActuator* alpha = &mb->MoveBlocks[AXIS_ALPHA];
    mb->DeepReset_ToDefault();

    switch (axis_name){
        case 'R':
            // Logger::Print("Twh2Row_G28_Runner::SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            // Logger::Print("Twh2Row_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            alpha->TargetPosition = 99999;
            alpha->Speed = 100;
            alpha->Acceleration = 0.05;
            break;

        default:
            Logger::Error(" Twh2Row_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("Twh2Row_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}


void Twh2Row_G28_Runner::SetHomedPosition(PositionTrigger* firer){
    Logger::Debug("Twh2Row_G28_Runner::SetHomedPosition()");
    if (this->_axis_name =='R'){
        FKPosition_XYZRPW fk;
        fk.X = 12.345;
        _arm_solution->SetCurrentPositionAs(&fk);
    }
}
