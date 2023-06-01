#include "gobot_house_robot.h"
#include "Robot/mcode_runner/mcode_os.h"

void GobotHouseRobot::Init(GobotMain_Board* board){
    Logger::Debug("Vsc_ArmSoution::Init()");

    this->_g28_runner=&this->g28_runner;
    g28_runner.Init(&mover, &arm_solution,"");
    this->_LinkMover(&mover);
    
    // gs_MoveBlock_Queue::Instance()._all_elements = this->__all_move_blocks;
}

void GobotHouseRobot::_InitStatic_Queues(){

}

void GobotHouseRobot::_Init_ArmSolution(){
    
}