#include "g28_Runner.h"
#include "sillicon_pump_arm_solution.h"

#define HOME_POSITION_FK_ROLL 12.345

void Silicon_Pump_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position){
    Logger::Info("Silicon_Pump_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;
}

void Silicon_Pump_G28_Runner::_SetMoveBlock_ToHome(char axis_name, MoveBlock* mb){
    Logger::Debug("Silicon_Pump_G28_Runner::_SetMoveBlock_ToHome()" );
    Serial.print(char(axis_name));
    Logger::Print("\taxis", char(axis_name));
    MoveBlock_SingleActuator* alpha = &mb->MoveBlocks[AXIS_ALPHA];
    mb->DeepReset_ToDefault();

    switch (axis_name){
        case 'X':
            // Logger::Print("Silicon_Pump_G28_Runner::_SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            // Logger::Print("Silicon_Pump_G28_Runner::_SetMoveBlock_ToHome()  point", 22);
            alpha->TargetPosition = 99999;
            alpha->Speed = 900;
            alpha->Acceleration = 0.5;
            break;

        default:
            Logger::Error(" Silicon_Pump_G28_Runner::_SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    // Logger::Print("Silicon_Pump_G28_Runner::_SetMoveBlock_ToHome()  point", 99);
}


void Silicon_Pump_G28_Runner::_SetHomedPosition(PositionTrigger* firer){
    Logger::Debug("Silicon_Pump_G28_Runner::_SetHomedPosition()");
    FKPosition_XYZRPW current_fk;
    IKPosition_abgdekl ik;
    if (this->_axis_name =='X'){
        current_fk.X = HOME_POSITION_FK_ROLL;
        _arm_solution->SetCurrentPositionAs(&current_fk);
        
        _arm_solution->FK_to_IK(&current_fk, &ik);
        _arm_solution->SetCurrentPositionAs(&ik);
    }
}
