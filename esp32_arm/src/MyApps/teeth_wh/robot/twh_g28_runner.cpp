#include "twh_g28_runner.h"
#include "twh_arm_solution.h"


void Twh_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution){
    Logger::Info("Twh_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;
}

void Twh_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("Twh_G28_Runner::SetMoveBlock_ToHome()" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* alpha;
    MoveBlock_SingleActuator* beta;
    mb->DeepReset_ToDefault();

    switch (axis){
        case 'Y':
            // Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            // Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            alpha = &mb->MoveBlocks[AXIS_ALPHA];
            alpha->TargetPosition = 99999;
            alpha->Speed = 10000;
            alpha->Acceleration = 99;
            beta = &mb->MoveBlocks[AXIS_BETA];
            beta->TargetPosition = -99999;
            beta->Speed = 10000;
            beta->Acceleration = 99;
            break;

        case 'a':
            // Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            // Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            alpha = &mb->MoveBlocks[AXIS_ALPHA];
            alpha->TargetPosition = -99999;
            alpha->Speed = 100;
            alpha->Acceleration = 0.05;
            beta = &mb->MoveBlocks[AXIS_BETA];
            beta->TargetPosition = -99999;
            beta->Speed = 100;
            beta->Acceleration = 0.05;
            break;

        default:
            Logger::Error(" Twh_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}


void Twh_G28_Runner::SetHomedPosition(PositionTrigger* firer){
    Logger::Debug("Twh_G28_Runner::SetHomedPosition()");
    if (this->_axis_name =='a'){
        // do nothing
    }else if (this->_axis_name == 'Y'){
        // must home('a') first, then home('X')
        // Logger::Print("Twh_G28_Runner::SetHomedPosition() Y", 31);
        MiddleKinematic mk;
        mk.Arm_Angle = DEG_TO_RAD * (-90);
        mk.Arm_shaft_At_Y= -190 + 3;
        // Logger::Print("Twh_G28_Runner::SetHomedPosition() Y", 32);
        FKPosition_XYZRPY fk;
        Twh_ArmSolution arm;
        // arm.LinkConfig();
        arm.real_MK_to_FK(&mk, &fk);
        // this->_arm_solution->MK_to_FK();
        // Logger::Print("Twh_G28_Runner::SetHomedPosition() Y", 33);

        IKPosition_abgdekl ik;
        this->_arm_solution->IK(&fk,&ik);
        // Logger::Print("Twh_G28_Runner::SetHomedPosition() Y", 34);
        this->_arm_solution->SetCurrentPositionAs(&ik);
        
    }
}
