#include "gobot_main_robot.h"
#include "Robot/mcode_runner/mcode_runners.h"
#include "CNC/mover/teensy_step_gateway.h"

StepControl step_control;

void GobotMainRobot::Init(GobotMain_Board* board){
    Logger::Debug("Vsc_ArmSoution::Init()");
    this->_cnc_board = board;
    this->_LinkEef(board->GetEef());

    this->__g28_runner=&this->g28_runner;
    // g28_runner.LinkMover(&mover);
    g28_runner.Init(&mover);
    this->LinkMover(&mover);
    
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    this->__planner.__arm_solution = &arm_solution;
    this->_arm_solution = &this->arm_solution;  



    // this->mover.LinkActuator('A', board->GetActuator(AXIS_ALPHA));
    TeensyStep_Gateway::Instance().Init(&step_control);
    TeensyStep_Gateway::Instance().AddStepper(board->GetStepper(AXIS_ALPHA));
    TeensyStep_Gateway::Instance().AddStepper(board->GetStepper(AXIS_BETA));


        // TODO: for mover config
    //     Vsc_ActuatorAlphaMechanic config;
    // this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
    // // this->_micro_steps_on_stepper_driver = 16;
    // // this->_motor_gear_teeth_count = 10;
    // // this->_slave_pulley_teeth_count = 90;
    // // this->_motor_step_angle_in_degree = 0.7003891050583658;
}