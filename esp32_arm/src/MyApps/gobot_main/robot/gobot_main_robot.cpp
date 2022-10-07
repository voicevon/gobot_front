#include "gobot_main_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
// #include "Robot/mcode_runner/mcode_runners.h"
#include "CNC/mover/teensy_step_gateway.h"
#include "CNC/Actuator/stepper/actuator_stepper_calculator.h"

StepControl step_control;

void GobotMainRobot::Init(GobotMain_Board* board){
    Logger::Debug("GobotMainRobot::Init()");
    this->_cnc_board = board;
    this->_LinkEef(board->GetEef());

    this->__g28_runner=&this->g28_runner;
    g28_runner.Init(&mover);
    this->LinkMover(&mover);
    
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    this->__planner.__arm_solution = &arm_solution;
    this->_arm_solution = &this->arm_solution;  

    TeensyStep_Gateway::Instance().Init(&step_control);
    TeensyStep_Gateway::Instance().AddStepper(board->GetStepper(AXIS_ALPHA));
    TeensyStep_Gateway::Instance().AddStepper(board->GetStepper(AXIS_BETA));
}

void GobotMainRobot::__InitActuator(GobotMain_Board* board){
    Logger::Info("GobotMainRobot::__InitActuator()");
    Actuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    Actuator_List::Instance().AddActuator(&__actuator_alpha);
    Actuator_List::Instance().AddActuator(&__actuator_beta);

    this->__actuator_alpha.MyName = 'a';
    this->__actuator_alpha.LinkStepper(board->GetStepper(AXIS_ALPHA), 1.0f);
    this->__actuator_beta.MyName = 'a';
    this->__actuator_beta.LinkStepper(board->GetStepper(AXIS_BETA), 1.0f);
    
    ActuatorStepper_Calculator helper;
    helper._micro_steps_on_stepper_driver = 16;
    helper._motor_gear_teeth_count = 10;
    helper._slave_pulley_teeth_count = 90;

    float slope = helper.GetActuatorToCncFormular_Slope_raw_per_mm();
    this->__actuator_alpha.Init_FomularSlope(slope);
    this->__actuator_beta.Init_FomularSlope(slope);



        // TODO: for mover config
    //     Vsc_ActuatorAlphaMechanic config;
    // this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
    // // this->_micro_steps_on_stepper_driver = 16;
    // // this->_motor_gear_teeth_count = 10;
    // // this->_slave_pulley_teeth_count = 90;
    // // this->_motor_step_angle_in_degree = 0.7003891050583658;
}
