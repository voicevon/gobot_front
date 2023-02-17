// #include "loop_porter_robot.h"
#include "sillicon_pump_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "MyLibs/component/display/digital_number/hc595_digital_number.h"


void Sillicon_Pump_Robot::MySpinOnce(){
    static int last_cnc_position = 0;
    FKPosition_XYZRPW current_fk;
    __arm_solution.GetRealTimePosition(&current_fk);

    if (last_cnc_position != (int)(current_fk.X)){
        Logger::Print("Sillicon_Pump_Robot::MySpinOnce()  cnc_position", current_fk.X);
        last_cnc_position = current_fk.X;
        // __board->GetDisplayer()->ShowNumber(last_cnc_position);
    }

}

void Sillicon_Pump_Robot::Init(Sillicon_Pump_Board* board){
    Logger::Debug("Sillicon_Pump_Robot::Init()");
    _InitStatic_Queues();
    __InitStatic_Actuators(board);
    _Init_ArmSolution();
    _LinkMover(&__mover);
    __board = board;
    _g28_runner = &__g28_runner;
    __g28_runner.Init(&__mover, &__arm_solution);
    McodeOS::Instance().LinkJsonLoader_ForM408Runner(&__json_loader_for_m408);
    __m42_runner_led_output.Init(board);
    McodeOS::Instance().LinkRunner(&__m42_runner_led_output);

}

void Sillicon_Pump_Robot::__InitStatic_Actuators(Sillicon_Pump_Board* board){
    Logger::Info("Sillicon_Pump_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    this->__actuator_alpha.MyName = 'a';
    __actuator_alpha.LinkStepper(board->GetStepper(EnumAxis_Inverseinematic::AXIS_ALPHA));

}

void Sillicon_Pump_Robot::_Init_ArmSolution(){
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


void Sillicon_Pump_Robot::_InitStatic_Queues(){
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
    line->PrintOUt("caller: Sillicon_Pump_Robot::_InitStatic_Queues()");
    Queue_LineSegment::Instance().Deposit();
    Logger::Print("Sillicon_Pump_Robot::Init", 83);
}


