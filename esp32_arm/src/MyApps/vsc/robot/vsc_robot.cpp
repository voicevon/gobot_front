#include "vsc_robot.h"

void VscRobot::Init(Vsc_Board* board){
    Logger::Info("Vsc_ArmSoution::Init() is entering...");
    this->_cnc_board = board;
    this->_LinkEef(board->GetEef());

    //  queue_move_block involved.
    this->__queue_move_block._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    arm_solution.__queue_move_block = &this->__queue_move_block;    //todo:  LinkAsProducer()
    this->__mover.__queue_move_block = &this->__queue_move_block;   //todo:  LinkAsConsumer()
    this->__planner.__arm_solution = &arm_solution;
    this->__mover.Init(board);

    arm_solution.Init(board);
    // this->arm_solution.planner = &this->__planner;
    arm_solution._mover_base = &this->__mover;
    this->_arm_solution = &this->arm_solution;  


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