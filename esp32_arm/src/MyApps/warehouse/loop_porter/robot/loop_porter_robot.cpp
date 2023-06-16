#include "loop_porter_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"
#include "von/cpp/mqtt/g_var.h"
// #include "MyLibs/component/display/digital_number/hc595_digital_number.h"
#include "Robot/board/display/digital_number/hc595_digital_number.h"


void Twh_LoopPorter_Robot::MySpinOnce(){
    static int last_cnc_position = 0;
    FKPosition_XYZRPW current_fk;
    __arm_solution.GetRealTimePosition(&current_fk);

    if (last_cnc_position != (int)(current_fk.X +0.5)){
        last_cnc_position = current_fk.X + 0.5;
        __board->GetNumberDisplayer()->ShowNumber(last_cnc_position);
        // Logger::Debug("Twh_LoopPorter_Robot::MySpinOnce()");
        // Logger::Print("cnc_position", current_fk.X);
        // Logger::Print("last_cnc_position", last_cnc_position);
    }

}

void Twh_LoopPorter_Robot::Init(Twh_LoopPorter_Board* board, const char* mqtt_topic_for_home_position){
    Logger::Debug("Twh_LoopPorter_Robot::Init()");
    _InitStatic_Queues();
    __InitStatic_Actuators(board);
    _Init_ArmSolution();
    _LinkMover(&__mover);
    __board = board;
    _g28_runner = &__g28_runner;
    __g28_runner.Init(&__mover, &__arm_solution, mqtt_topic_for_home_position);
    McodeOS::Instance().LinkJsonLoader_ForM408Runner(&__json_loader_for_m408);
    __m42_runner_led_output.Init(board);
    McodeOS::Instance().LinkRunner(&__m42_runner_led_output);

}

void Twh_LoopPorter_Robot::__InitStatic_Actuators(Twh_LoopPorter_Board* board){
    Logger::Info("Twh_LoopPorter_Robot::Init() Actuators.");
    gs_CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    gs_CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    this->__actuator_alpha.MyName = 'a';
    __actuator_alpha.LinkStepper(board->GetStepper());

}

void Twh_LoopPorter_Robot::_Init_ArmSolution(){
    this->_LinkArmSolution_for_planner(&__arm_solution);
    // We don't care the value of current position,
    // But  fk_position and ik_position must be consistent
    FKPosition_XYZRPW current_fk;
    current_fk.Roll = 0;
    current_fk.X = 0;
    __arm_solution.SetCurrentPositionAs(&current_fk);

    IKPosition_abgdekl ik;
    __arm_solution.FK_to_IK(&current_fk, &ik);
    __arm_solution.SetCurrentPositionAs(&ik);
}


void Twh_LoopPorter_Robot::_InitStatic_Queues(){
    // gs_MoveBlock_Queue::Instance()._all_elements = this->__all_move_blocks;
    // Init LineSegment queue head
    // gs_LineSegment_Queue::Instance()._all_elements = this->__all_line_segments;
    LineSegment* line = gs_LineSegment_Queue::Instance().GetRoom_ForDeposit();
    line->TargetPosition.X = 0;
    line->TargetPosition.Y = 0;
    line->TargetPosition.Z = 0;
    line->TargetPosition.Roll = 0;
    line->TargetPosition.Pitch = 0;
    line->TargetPosition.Yaw = 0;
    // line->PrintOUt("caller: Twh_LoopPorter_Robot::_InitStatic_Queues()");
    gs_LineSegment_Queue::Instance().Deposit();
    Logger::Print("Twh_LoopPorter_Robot::Init()  point  99", 99);
}


