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
            Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            // Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            alpha = &mb->MoveBlocks[AXIS_ALPHA];
            // alpha->IsAbsTargetPosition = false;
            alpha->TargetPosition = -99999;
            alpha->Speed = 10000;
            alpha->Acceleration = 99;
            beta = &mb->MoveBlocks[AXIS_BETA];
            // beta->IsAbsTargetPosition = false;
            beta->TargetPosition = 99999;
            beta->Speed = 10000;
            beta->Acceleration = 99;
            break;

        case 'a':
            Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            // Logger::Print("Twh_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            alpha = &mb->MoveBlocks[AXIS_ALPHA];
            // alpha->IsAbsTargetPosition = false;
            alpha->TargetPosition = 99999;
            alpha->Speed = 100;
            alpha->Acceleration = 0.05;
            beta = &mb->MoveBlocks[AXIS_BETA];
            // beta->IsAbsTargetPosition = false;
            beta->TargetPosition = 99999;
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
    if (this->_axis_name =='a'){
        // do nothing
    }else if (this->_axis_name == 'Y'){
        // must home('a') first, then home('X')
        MiddleKinematic mk;
        mk.Arm_Angle = DEG_TO_RAD * (-90);
        mk.Arm_shaft_At_Y= -188;
        FKPosition_XYZRPY fk;
        Twh_ArmSolution arm;
        arm.real_MK_to_FK(&mk, &fk);

        IKPosition_abgdekl ik;
        this->_arm_solution->IK(&fk,&ik);
        // ik.Positions[AXIS_ALPHA] = 123;
        // ik.Positions[AXIS_BETA] = 123;
        this->_arm_solution->SetCurrentPosition(&ik);
        
        bool debug = true;
        if (debug){
            FKPosition_XYZRPY fk;
            this->_arm_solution->FK(&ik, &fk);
            fk.PrintOut("Twh_G28_Runner::SetHomedPosition()");
        }
    }
}
