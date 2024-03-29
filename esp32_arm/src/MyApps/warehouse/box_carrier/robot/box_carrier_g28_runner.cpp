#include "box_carrier_g28_runner.h"


void BoxCarrier_G28_Runner::Init( CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position){
    Logger::Info("BoxCarrier_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;

    // homing.IsDirectionToMax = true;
    // homing.Speed = TWO_PI / 60;  // 60 seconds Per loop.
    // homing.Accelleration = 0.1;
    // homing.DistanceToGo = TWO_PI;
    
    Logger::Info("BoxCarrier_G28_Runner::Init() Alpha axis home_triggers");
    // PositionTrigger* trigger;
    // trigger = gs_PositionTrigger_Array::Instance().GetPositionTrigger(0);
    // trigger = board->GetPositionTrigger(0);
    // trigger->SetTriggerPosition(TWO_PI* 1 / 386);      // @01 pitch, total 386 pitches,
    // trigger->AxisName  = 'X';    
    // __homer.AppendPositionTrigger(trigger);

    // trigger = board->GetPositionTrigger(1);
    // trigger = gs_PositionTrigger_Array::Instance().GetPositionTrigger(1);
    // trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
    // trigger->AxisName='Z';
    // __homer.AppendPositionTrigger(trigger);
}


void BoxCarrier_G28_Runner::_SetMoveBlock_ToHome(char axis, MoveBlock* mb){
    Logger::Debug("BoxCarrier_G28_Runner::_SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case AXIS_X:
            Logger::Print("BoxCarrier_G28_Runner::_SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("BoxCarrier_G28_Runner::_SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            Logger::Print("BoxCarrier_G28_Runner::_SetMoveBlock_ToHome()  point", 23);
            // move->IsAbsTargetPosition = false;
            move->TargetPosition = 123;
            move->Speed = 2.34;
            move->Acceleration = 0.4;
            break;
        default:
            Logger::Error(" BoxCarrier_G28_Runner::_SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("BoxCarrier_G28_Runner::_SetMoveBlock_ToHome()  point", 99);
}

