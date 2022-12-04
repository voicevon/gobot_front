#include "twh_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"



void TeechWarehouse_Robot::Init(TeethWarehouse_Board* board){
    Logger::Debug("TeechWarehouse_Robot::Init()");
    this->_InitStatic_Queues();
    this->__InitStatic_Actuators(board);
    this->_Init_ArmSolution();

    this->_LinkMover(&__mover);
    
    this->_g28_runner = &this->__g28_runner;
    __g28_runner.Init(&__mover, &__arm_solution);

    this->__m42_runner.Init(board);
    McodeOS::Instance().LinkRunner(&this->__m42_runner);

}

void TeechWarehouse_Robot::__InitStatic_Actuators(TeethWarehouse_Board* board){
    Logger::Info("TeechWarehouse_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    CncActuator_List::Instance().AddActuator(&__actuator_beta);
    CncActuator_List::Instance().AddActuator(&__actuator_gamma);

    this->__actuator_alpha.MyName = 'a';
    this->__actuator_beta.MyName = 'b';
    this->__actuator_gamma.MyName = 'g';
    
    __actuator_alpha.LinkStepper(board->GetStepper_Alpha());
    __actuator_beta.LinkStepper(board->GetStepper_Beta());

    // Gear:  M=0.8, T=48, circle_length = 3.14*48*0.8=120.6mm
    // mm_per_degree = 120.6/360 = 0.335
    // max: Z=0,  degree=0
    // min: Z=-90.45, degree=270.
    // float slope = -0.335;
    // float offset = -270.0f;
    __actuator_gamma.LinkServo(board->GetServo_onVertical());
    // __actuator_gamma.Formular_SetSlope(slope);
    // __actuator_gamma.Formular_SetRawOffset(offset);
}

void TeechWarehouse_Robot::_Init_ArmSolution(){
    // __arm_config.arm_length = 205;
    // __arm_config.slave_gear_circle_length = 123.98;
    // __arm_config.master_slope_steps_per_mm = 12.778;
    // __arm_solution.LinkConfig(&__arm_config);
    this->_LinkArmSolution_for_planner(&__arm_solution);
}


void TeechWarehouse_Robot::_InitStatic_Queues(){
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
    line->PrintOUt();
    Queue_LineSegment::Instance().Deposit();
    Logger::Print("TeechWarehouse_Robot::Init", 83);
}
