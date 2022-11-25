#include "arm_solution_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
#include "MyLibs/message_queue.h"

void ArmSolutionBase::ForceStopMover(){
	// this->_mover_base->AllActuatorsStop();

	//TODO:    How to do it without mover_base??
}

void ArmSolutionBase::GetRealTimePosition(FKPosition_XYZRPY* position_fk){
	//TODO::  
	IKPosition_abgdekl realtime_ik;
	// realtime_ik.alpha = CncActuator_List::Instance().GetActuator(AXIS_ALPHA)->GetCurrentPosition();
	realtime_ik.Actuator[AXIS_ALPHA] = CncActuator_List::Instance().GetActuator(AXIS_ALPHA)->GetCurrentPosition();
	this->FK(&realtime_ik, position_fk);
}

void ArmSolutionBase::SetCurrentPosition(FKPosition_XYZRPY* fk_position){

}

void ArmSolutionBase::SetCurrentPosition(IKPosition_abgdekl* ik_position){

}
