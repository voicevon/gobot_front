#include "Twh_G28_Runner.h"
#include "../twh_xa_arm_solution.h"


void Twh1_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position){
    Logger::Info("Twh1_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;
}

void Twh1_G28_Runner::_SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("Twh1_G28_Runner::_SetMoveBlock_ToHome()" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* alpha = &mb->MoveBlocks[AXIS_ALPHA];
    MoveBlock_SingleActuator* beta = &mb->MoveBlocks[AXIS_BETA];
    MoveBlock_SingleActuator* gamma = &mb->MoveBlocks[AXIS_GAMMA];
    mb->DeepReset_ToDefault();

    switch (axis){
        case 'Y':
            // Logger::Print("Twh1_G28_Runner::_SetMoveBlock_ToHome()  point", 21);
            // Logger::Print("Twh1_G28_Runner::_SetMoveBlock_ToHome()  point", 22);
            alpha->TargetPosition = -99999;
            alpha->Speed = 10000;
            alpha->Acceleration = 99;
            beta->TargetPosition = 99999;
            beta->Speed = 10000;
            beta->Acceleration = 99;
            gamma ->TargetPosition = 260;
            break;

        case 'a':
            // Logger::Print("Twh1_G28_Runner::_SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            // Logger::Print("Twh1_G28_Runner::_SetMoveBlock_ToHome()  point", 22);
            alpha->TargetPosition = 99999;
            alpha->Speed = 100;
            alpha->Acceleration = 0.05;
            beta->TargetPosition = 99999;
            beta->Speed = 100;
            beta->Acceleration = 0.05;
            gamma ->TargetPosition = 260;
            break;

        default:
            Logger::Error(" Twh1_G28_Runner::_SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("Twh1_G28_Runner::_SetMoveBlock_ToHome()  point", 99);
}


void Twh1_G28_Runner::_SetHomedPosition(PositionTrigger* firer){
    Logger::Debug("Twh1_G28_Runner::_SetHomedPosition()");
    if (this->_axis_name =='a'){
        // do nothing
    }else if (this->_axis_name == 'Y'){
        // must home('a') first, then home('Y')
        // Logger::Print("Twh1_G28_Runner::_SetHomedPosition() Y", 31);
        MiddleKinematic mk;
        mk.Arm_Angle = DEG_TO_RAD * (-90);
        mk.Arm_shaft_At_Y= 0;
        // Logger::Print("Twh1_G28_Runner::_SetHomedPosition() Y", 32);
        Twh1_XaArmSolution arm;
        FKPosition_XYZRPW fk;
        arm.real_MK_to_FK(&mk, &fk);
        this->_arm_solution->SetCurrentPositionAs(&fk);
        // Logger::Print("Twh1_G28_Runner::_SetHomedPosition() Y", 33);

        IKPosition_abgdekl ik;
        arm.real_MK_to_IK(&mk, &ik);
        this->_arm_solution->SetCurrentPositionAs(&ik);
        // Logger::Print("Twh1_G28_Runner::_SetHomedPosition() Y", 34);
        
    }
}
