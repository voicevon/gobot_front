#include "teeth_wh_g28_runner.h"


void TeethWarehouse_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution){
    Logger::Info("TeethWarehouse_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;

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


void TeethWarehouse_G28_Runner::SetHomedPosition(float triggered_position){
    if (this->_axis_name =='a'){
        // do nothing
    }else if (this->_axis_name == 'X'){
        // must home('a') first, then home('X')
        IKPosition_abgdekl ik;
        ik.Actuator[AXIS_ALPHA] = 123;
        ik.Actuator[AXIS_BETA] = 123;
        this->_arm_solution->SetCurrentPosition(&ik);
        
        bool debug = true;
        if (debug){
            FKPosition_XYZRPY fk;
            this->_arm_solution->FK(&ik, &fk);
            fk.PrintOut("TeethWarehouse_G28_Runner::SetHomedPosition() 'X'");
        }
    }
}
