#include "twh2_g28_runner.h"
#include "../arm_solution_core_xz/twh2_xz_arm_solution.h"


void Twh2_G28_Runner_CoreXz::Init(CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position){
    Logger::Info("Twh2_G28_Runner_CoreXz::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;
}

void Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome()" );
    Serial.print(axis);
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* alpha = &mb->MoveBlocks[AXIS_ALPHA];
    MoveBlock_SingleActuator* beta = &mb->MoveBlocks[AXIS_BETA];
    mb->DeepReset_ToDefault();

    switch (axis){
        case 'X':
            // Logger::Print("Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome()  point", 21);
            // Logger::Print("Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome()  point", 22);
            alpha->TargetPosition = -99999;
            alpha->Speed = 10000;
            alpha->Acceleration = 99;
            beta->TargetPosition = 99999;
            beta->Speed = 10000;
            beta->Acceleration = 99;
            break;

        case 'Z':
            // Logger::Print("Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            // Logger::Print("Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome()  point", 22);
            alpha->TargetPosition = 99999;
            alpha->Speed = 100;
            alpha->Acceleration = 0.05;
            beta->TargetPosition = 99999;
            beta->Speed = 100;
            beta->Acceleration = 0.05;
            break;

        default:
            Logger::Error(" Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome() Unknown axis");
            Serial.println(axis);
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("Twh2_G28_Runner_CoreXz::_SetMoveBlock_ToHome()  point", 99);
}


void Twh2_G28_Runner_CoreXz::_SetHomedPosition(PositionTrigger* firer){
    Logger::Debug("Twh2_G28_Runner_CoreXz::_SetHomedPosition()");
    if (this->_axis_name =='Z'){
        // do nothing
    }else if (this->_axis_name == 'X'){
        // Logger::Print("Twh2_G28_Runner_CoreXz::_SetHomedPosition() Y", 31);
        MiddleKinematic mk;
        mk.Arm_Angle = DEG_TO_RAD * (-90);
        mk.Arm_shaft_At_Y= 0;
        // Logger::Print("Twh2_G28_Runner_CoreXz::_SetHomedPosition() Y", 32);
        Twh2_XzArmSolution arm;
        FKPosition_XYZRPW fk;
        arm.real_MK_to_FK(&mk, &fk);
        this->_arm_solution->SetCurrentPositionAs(&fk);
        // Logger::Print("Twh2_G28_Runner_CoreXz::_SetHomedPosition() Y", 33);

        IKPosition_abgdekl ik;
        arm.real_MK_to_IK(&mk, &ik);
        this->_arm_solution->SetCurrentPositionAs(&ik);
        // Logger::Print("Twh2_G28_Runner_CoreXz::_SetHomedPosition() Y", 34);
        
    }
}
