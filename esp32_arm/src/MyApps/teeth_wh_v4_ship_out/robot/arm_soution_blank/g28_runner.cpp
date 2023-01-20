#include "g28_Runner.h"
#include "blank_arm_solution.h"

#define HOME_POSITION_FK_ROLL 12.345

void Twh4_Shipout_G28_Runner::Init(CncMover* mover, ArmSolutionBase* arm_solution){
    Logger::Info("Twh4_Shipout_G28_Runner::Init() Hoiming_config");
    this->_mover = mover;
    this->_arm_solution = arm_solution;
}

void Twh4_Shipout_G28_Runner::SetMoveBlock_ToHome(char axis_name, MoveBlock* mb){
    Logger::Debug("Twh4_Shipout_G28_Runner::SetMoveBlock_ToHome()" );
}


void Twh4_Shipout_G28_Runner::SetHomedPosition(PositionTrigger* firer){
    Logger::Debug("Twh4_Shipout_G28_Runner::SetHomedPosition()");
}
