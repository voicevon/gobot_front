#include "vsc_g28_runner.h"


void Vsc_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution){
    Logger::Info("Vsc_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;

}

void Vsc_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("Vsc_G28_Runner::SetMoveBlock_ToHome()" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case 'X':
            // Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            // Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            // Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            // move->IsAbsTargetPosition = false;
            move->TargetPosition = TWO_PI;
            move->Speed = 0.1;
            move->Acceleration = 0.05;
            break;
        default:
            Logger::Error(" Vsc_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("Vsc_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}


void Vsc_G28_Runner::SetHomedPosition(PositionTrigger* firer){
    if (this->_axis_name =='a'){
        // do nothing
    }else if (this->_axis_name == 'X'){
        // must home('a') first, then home('X')
        IKPosition_abgdekl ik;
        ik.Positions[AXIS_ALPHA] = 123;
        ik.Positions[AXIS_BETA] = 123;
        this->_arm_solution->SetCurrentPositionAs(&ik);
        
        bool debug = true;
        if (debug){
            FKPosition_XYZRPW fk;
            this->_arm_solution->IK_to_FK(&ik, &fk);
            fk.PrintOut("Twh_G28_Runner::SetHomedPosition() 'X'");
        }
    }
}
