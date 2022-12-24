#include "twh_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"



void Twh2_Robot::Init(Twh2_Board* board){
    Logger::Debug("Twh2_Robot::Init()");
    this->_InitStatic_Queues();
    this->__InitStatic_Actuators(board);
    this->_Init_ArmSolution();

    this->_LinkMover(&__mover);
    
    this->_g28_runner = &this->__g28_runner_corexz;
    __g28_runner_corexz.Init(&__mover, &__arm_solution_corexz);

    this->__m42_runner.Init(board);
    McodeOS::Instance().LinkRunner(&this->__m42_runner);

}

void Twh2_Robot::__InitStatic_Actuators(Twh2_Board* board){
    Logger::Info("Twh2_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    CncActuator_List::Instance().AddActuator(&__actuator_beta);
    // CncActuator_List::Instance().AddActuator(&__actuator_gamma);

    this->__actuator_alpha.MyName = 'a';
    this->__actuator_beta.MyName = 'b';
    // this->__actuator_gamma.MyName = 'g';
    
    __actuator_alpha.LinkStepper(board->GetStepper_Alpha());
    __actuator_beta.LinkStepper(board->GetStepper_Beta());

    // __actuator_gamma.LinkServo(board->GetServo_onVertical());

}

void Twh2_Robot::_Init_ArmSolution(){
    this->_LinkArmSolution_for_planner(&__arm_solution_corexz);
    // We don't care the value of current position, But  fk_position and ik_position must be consistent
    FKPosition_XYZRPW current;
    current.X = 0;
    current.Y = 0;
    current.Z = 0;
    __arm_solution_corexz.SetCurrentPositionAs(&current);
    IKPosition_abgdekl ik;

    __arm_solution_corexz.FK_to_IK(&current, &ik);
    __arm_solution_corexz.SetCurrentPositionAs(&ik);
}


void Twh2_Robot::_InitStatic_Queues(){
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
    line->PrintOUt("caller: Twh2_Robot::_InitStatic_Queues()");
    Queue_LineSegment::Instance().Deposit();
    Logger::Print("Twh2_Robot::Init", 83);
}
