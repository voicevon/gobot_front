#include "sillicon_pump_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "MyLibs/component/display/digital_number/hc595_digital_number.h"


void Silicon_Pump_Robot::MySpinOnce(){


}

void Silicon_Pump_Robot::Init(Silicon_Pump_Board* board){
    Logger::Debug("Silicon_Pump_Robot::Init()");
    _InitStatic_Queues();
    __InitStatic_Actuators(board);
    _Init_ArmSolution();
    _LinkMover(&__mover);
    __board = board;
    _g28_runner = &__g28_runner;
    __g28_runner.Init(&__mover, &__arm_solution,"");
    McodeOS::Instance().LinkJsonLoader_ForM408Runner(&__json_loader_for_m408);
    __m42_runner_led_output.Init(board);
    McodeOS::Instance().LinkRunner(&__m42_runner_led_output);

}

void Silicon_Pump_Robot::__InitStatic_Actuators(Silicon_Pump_Board* board){
    Logger::Info("Silicon_Pump_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    CncActuator_List::Instance().AddActuator(&__actuator_beta);
    CncActuator_List::Instance().AddActuator(&__actuator_gamma);
    __actuator_alpha.MyName = 'a';
    __actuator_beta.MyName = 'b';
    __actuator_beta.MyName = 'g';

    __actuator_alpha.LinkStepper(board->GetStepper(EnumAxis_Inverseinematic::AXIS_ALPHA));
    __actuator_beta.LinkStepper(board->GetStepper(EnumAxis_Inverseinematic::AXIS_BETA));
    __actuator_gamma.LinkStepper(board->GetStepper(EnumAxis_Inverseinematic::AXIS_GAMMA));

}

void Silicon_Pump_Robot::_Init_ArmSolution(){
    this->_LinkArmSolution_for_planner(&__arm_solution);
    // We don't care the value of current position,
    // But  fk_position and ik_position must be consistent
    FKPosition_XYZRPW current;
    current.X =0;
    current.Y = 0;
    __arm_solution.SetCurrentPositionAs(&current);

    IKPosition_abgdekl ik;
    __arm_solution.FK_to_IK(&current, &ik);
    __arm_solution.SetCurrentPositionAs(&ik);
}


void Silicon_Pump_Robot::_InitStatic_Queues(){
    gs_MoveBlock_Queue::Instance().all_elements = this->__all_move_blocks;
    // Init LineSegment queue head
    gs_LineSegment_Queue::Instance().all_elements =  this->__all_line_segments;
    LineSegment* line = gs_LineSegment_Queue::Instance().GetRoom();
    line->TargetPosition.X = 0;
    line->TargetPosition.Y = 0;
    line->TargetPosition.Z = 0;
    line->TargetPosition.Roll = 0;
    line->TargetPosition.Pitch = 0;
    line->TargetPosition.Yaw = 0;
    line->PrintOUt("caller: Silicon_Pump_Robot::_InitStatic_Queues()");
    gs_LineSegment_Queue::Instance().Deposit();
    Logger::Print("Silicon_Pump_Robot::Init", 83);
}


