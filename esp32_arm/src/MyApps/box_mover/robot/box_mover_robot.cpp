#include "box_mover_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
// #include "Robot/mcode_runner/mcode_runners.h"

void BoxMoverRobot::Init(Board2205Cnc* board){
    Logger::Debug("Vsc_ArmSoution::Init()");
    // this->_cnc_board = board;
    // this->_LinkEef(board->GetEef());

    this->_g28_runner=&this->g28_runner;
    // g28_runner._LinkMover(&mover);
    g28_runner.Init(&mover);
    this->_LinkMover(&mover);
    
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    // this->__planner.__arm_solution = &arm_solution;
    // this->_arm_solution = &this->arm_solution;  

    // Logger::Info("Vsc_ArmSoution::Init() Pid controller.");
    // this->__all_pids.AppendPidController(&this->__speed_pid);
    //So all pid controllers are configable via mcode. example: 'M130 N0 P1 I2 D3'
    // M130_Runner* m130 = (M130_Runner*) McodeOS::getInstance().GetRunner(130);
    // m130->LinkPidControllers(&this->__all_pids);
    
    // motor is the user of PID controller
    // board->LinkSpeedPid_ForMotor(&this->__speed_pid);
    // and initial setting
    // this->__speed_pid.P = 100;
    // this->__speed_pid.I = 100;
    // this->__speed_pid.D = 0;


    // this->mover.LinkActuator('A', board->GetActuator(AXIS_ALPHA));

        // TODO: for mover config
    //     Vsc_ActuatorAlphaMechanic config;
    // this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
    // // this->_micro_steps_on_stepper_driver = 16;
    // // this->_motor_gear_teeth_count = 10;
    // // this->_slave_pulley_teeth_count = 90;
    // // this->motor_step_angle_in_degree = 0.7003891050583658;
}