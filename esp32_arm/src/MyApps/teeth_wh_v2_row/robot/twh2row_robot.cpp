#include "twh2row_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"
#include "IoT/wifi_mqtt_client.h"


// TODO:  consider:  if gcode is move_to zero distance.  
// Solution:  report state by M402 command followed any G1-code. 
void Twh2Row_Robot::MySpinOnce(){
    switch (__current_state){
        case 0:
            if (_mover->IsMoving()){
                __current_state = 1;
            }
            break;
        case 1:
            if (!_mover->IsMoving()){
                // publish ready  //report state topic =  "twh/221109/r0/state"
                g_mqttClient.publish("twh/221109/r0/state", 2, true, "ready");
                __current_state = 2;
            }
            break;
        case 2:
                //__current_state should be set to moving when start G1-code
                // No need to report via MQTT.
            break;
        default:
            __current_state = 0;
            break;
    }
}

void Twh2Row_Robot::Init(Twh2Row_Board* board){
    Logger::Debug("Twh2Row_Robot::Init()");
    this->_InitStatic_Queues();
    this->__InitStatic_Actuators(board);
    this->_Init_ArmSolution();

    this->_LinkMover(&__mover);
    
    this->_g28_runner = &this->__g28_runner;
    __g28_runner.Init(&__mover, &__arm_solution);
    McodeOS::Instance().LinkJsonLoader_ForM408Runner(&__json_loader_for_m408);

}

void Twh2Row_Robot::__InitStatic_Actuators(Twh2Row_Board* board){
    Logger::Info("Twh2Row_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    this->__actuator_alpha.MyName = 'a';
    __actuator_alpha.LinkStepper(board->GetStepper());

}

void Twh2Row_Robot::_Init_ArmSolution(){
    this->_LinkArmSolution_for_planner(&__arm_solution);
    // We don't care the value of current position,
    // But  fk_position and ik_position must be consistent
    FKPosition_XYZRPW current;
    current.Roll = 0;
    __arm_solution.SetCurrentPositionAs(&current);

    IKPosition_abgdekl ik;
    __arm_solution.FK_to_IK(&current, &ik);
    __arm_solution.SetCurrentPositionAs(&ik);
}


void Twh2Row_Robot::_InitStatic_Queues(){
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    // Init LineSegment queue head
    Queue_LineSegment::Instance()._all_queue_ables = (Queue_able*) this->__all_line_segments;
    LineSegment* line = Queue_LineSegment::Instance().GetRoom();
    line->TargetPosition.X = 0;
    line->TargetPosition.Y = 0;
    line->TargetPosition.Z = 0;
    line->TargetPosition.Roll = 0;
    line->TargetPosition.Pitch = 0;
    line->TargetPosition.Yaw = 0;
    line->PrintOUt("caller: Twh2Row_Robot::_InitStatic_Queues()");
    Queue_LineSegment::Instance().Deposit();
    Logger::Print("Twh2Row_Robot::Init", 83);
}


