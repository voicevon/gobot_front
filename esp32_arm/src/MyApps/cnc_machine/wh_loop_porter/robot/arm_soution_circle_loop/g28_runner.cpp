#include "g28_Runner.h"
#include "circle_loop_arm_solution.h"

// #define HOME_POSITION_FK_ROLL 29.75

void Twh_LoopPorter_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution,const char* mqtt_topic_for_home_position ){
    Logger::Info("Twh_LoopPorter_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;
    MqttSubscriberManager::GetInstance().AddSubscriber(mqtt_topic_for_home_position, &__remote_home_position);
}

void Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome(char axis_name, MoveBlock* mb){
    Logger::Debug("Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()" );
    Logger::Print("\taxis", char(axis_name));
    MoveBlock_SingleActuator* alpha = &mb->MoveBlocks[AXIS_ALPHA];
    Logger::Print("Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()  point", 11 );
    mb->PrintOut("Test mb is none or not.  Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()");
    Serial.print("444444444444444444444");
    // mb->DeepReset_ToDefault();
    Serial.print("5555555555555555555");
    Logger::Print("Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()  point", 12 );

    switch (axis_name){
        case 'X':
            Logger::Print("Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()  point", 31);
            mb->DeepReset_ToDefault();
            Logger::Print("Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()  point", 32);
            alpha->TargetPosition = 511000;
            alpha->Speed = 900;
            alpha->Acceleration = 3000;
            break;

        default:
            Logger::Error(" Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome() Unknown axis");
            Logger::Halt("AcDc::TNT");
            break;
    }
    Logger::Print("Twh_LoopPorter_G28_Runner::_SetMoveBlock_ToHome()  point", 99);
}

void Twh_LoopPorter_G28_Runner::_InitHomePosition(char axis_name, float setting_position){
    // if (axis_name=='X'){
    //     __Setting_HOME_POSITION_FK_ROLL =  setting_position;
    // }else{
    //     Logger::Warn("Twh_LoopPorter_G28_Runner::InitHomePosition()");
    //     Logger::Print("Unknown axis_name = " , axis_name);
    // }
}

void Twh_LoopPorter_G28_Runner::_SetHomedPosition(PositionTrigger* firer){
    Logger::Debug("Twh_LoopPorter_G28_Runner::_SetHomedPosition()");
    FKPosition_XYZRPW current_fk;
    IKPosition_abgdekl ik;
    if (this->_axis_name =='X'){
        current_fk.X = __remote_home_position.Get();
        _arm_solution->SetCurrentPositionAs(&current_fk);
        
        _arm_solution->FK_to_IK(&current_fk, &ik);
        _arm_solution->SetCurrentPositionAs(&ik);
    }
}
