#include "vsc_robot.h"

void VscRobot::Init(Vsc_Board* board,  Vsc_ArmSoution *arm_solution){
    Logger::Info("Vsc_ArmSoution::Init() is entering...");
    this->_cnc_board = board;
    this->_LinkEef(board->GetEef());
    this->__queue_move_block._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    this->__planner.__moveblock_queue = &this->__queue_move_block;
    this->__mover.__moveblock_queue = &this->__queue_move_block;

    this->_arm_solution = arm_solution;

    this->_arm_solution->_cnc_board = board;
    this->_arm_solution->planner = &this->__planner;
    
    Logger::Info("Vsc_ArmSoution::Init() Pid controller.");
    //So all pid controllers are configable via mcode. example: 'M130 N0 P1 I2 D3'
    this->__all_pids.AppendPidController(&this->__speed_pid);
    this->_LinkPidControllers_M130(&this->__all_pids);
    // motor is the user of PID controller
    board->LinkSpeedPid_ForMotor(&this->__speed_pid);
    // and initial setting
    this->__speed_pid.P = 100;
    this->__speed_pid.I = 100;
    this->__speed_pid.D = 0;
}