#include "box_carrier_robot.h"
#include "Robot/mcode_runner/mcode_os.h"

void BoxCarrierRobot::Init(Board2204Cnc* board){
    Logger::Debug("Vsc_ArmSoution::Init()");
    // this->_cnc_board = board;
    // this->_LinkEef(board->GetEef());

    this->_g28_runner=&this->g28_runner;
    g28_runner.Init(&mover, &arm_solution,"");
    this->_LinkMover(&mover);
    
    gs_MoveBlock_Queue::Instance().all_elements = this->__all_move_blocks;
    // this->__planner.__arm_solution = &arm_solution;
    // this->_arm_solution = &this->arm_solution;  



    // this->mover.LinkActuator('A', board->GetActuator(AXIS_ALPHA));

        // TODO: for mover config
    //     Vsc_ActuatorAlphaMechanic config;
    // this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
    // // this->_micro_steps_on_stepper_driver = 16;
    // // this->_motor_gear_teeth_count = 10;
    // // this->_slave_pulley_teeth_count = 90;
    // // this->motor_step_angle_in_degree = 0.7003891050583658;
}