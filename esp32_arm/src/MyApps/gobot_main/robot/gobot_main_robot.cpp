#include "gobot_main_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/mover/teensy_step_gateway.h"
#include "CNC/Actuator/stepper/actuator_stepper_calculator.h"

StepControl step_control;

void GobotMainRobot::Init(GobotMain_Board* board){
    Logger::Debug("GobotMainRobot::Init()");
    // _cnc_board = board;
    // this->_LinkEef(board->GetEef());

    this->_g28_runner=&this->g28_runner;
    g28_runner.Init(&mover, &arm_solution);
    this->_LinkMover(&mover);
    
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    // this->__planner.__arm_solution = &arm_solution;
    // this->_arm_solution = &this->arm_solution;  

    TeensyStep_Gateway::Instance().Init(&step_control);
    TeensyStep_Gateway::Instance().AddStepper(board->GetStepper(AXIS_ALPHA));
    TeensyStep_Gateway::Instance().AddStepper(board->GetStepper(AXIS_BETA));
}

void GobotMainRobot::__InitActuator(GobotMain_Board* board){
    Logger::Info("GobotMainRobot::__InitActuator()");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    CncActuator_List::Instance().AddActuator(&__actuator_beta);

    this->__actuator_alpha.MyName = 'a';
    this->__actuator_alpha.LinkStepper(board->GetStepper(AXIS_ALPHA), 1.0f);
    this->__actuator_beta.MyName = 'a';
    this->__actuator_beta.LinkStepper(board->GetStepper(AXIS_BETA), 1.0f);
    
    ActuatorStepper_Calculator helper;
    helper.motor_driver_micro_steps = 16;
    helper.motor_gear_teeth_count = 10;
    helper.slave_pulley_teeth_count = 90;

    float slope = helper.Get_Formular_Slope_steps_per_mm();
    this->__actuator_alpha.Formular_SetSlope(slope);
    this->__actuator_beta.Formular_SetSlope(slope);



        // TODO: for mover config
    //     Vsc_ActuatorAlphaMechanic config;
    // this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
    // // this->_micro_steps_on_stepper_driver = 16;
    // // this->_motor_gear_teeth_count = 10;
    // // this->_slave_pulley_teeth_count = 90;
    // // this->motor_step_angle_in_degree = 0.7003891050583658;
}


void GobotMainRobot::_Init_ArmSolution(){
    // __arm_config.arm_length = 205;
    // // __arm_config.slave_gear_circle_length = 123.98;
    // __arm_config.master_slope_steps_per_mm = 12.778;
    // __arm_solution.LinkConfig(&__arm_config);
    // this->_LinkArmSolution(&__arm_solution);
}


void GobotMainRobot::_InitStatic_Queues(){
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