#include "robot.h"

void VscRobot::Init(Vsc_Board* board){
    Logger::Info("Vsc_CncSoution::Init() Pid controller.");
    //So all pid controllers are configable via mcode. example: 'M130 N0 P1 I2 D3'
    this->__all_pids.AppendPidController(&this->__speed_pid);
    this->_LinkPidControllers_M130(&this->__all_pids);
    // motor is the user of PID controller
    board->LinkSpeedPid_ForMotor(&this->__speed_pid);
    // and initial setting
    this->__speed_pid.P = 100;
    this->__speed_pid.I = 100;
    this->__speed_pid.D = 0;
    this->_LinkEef(board->GetEef());
}