#include "gobot_main_g28_runner.h"


void GobotMain_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution){
    Logger::Info("GobotMain_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;

}

void GobotMain_G28_Runner::SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("GobotMain_G28_Runner::SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case AXIS_X:
            Logger::Print("GobotMain_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("GobotMain_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            Logger::Print("GobotMain_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            // move->IsAbsTargetPosition = false;
            // move->TargetPosition = homing.DistanceToGo;
            // move->Speed = homing.Speed;
            // move->Acceleration = homing.Accelleration;
            break;
        default:
            Logger::Error(" GobotMain_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("GobotMain_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}


void GobotMain_G28_Runner::SetHomedPosition(PositionTrigger* firer){
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
