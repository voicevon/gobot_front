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
	realtime_ik.Positions[AXIS_ALPHA] = CncActuator_List::Instance().GetActuator(AXIS_ALPHA)->GetCurrentPosition();
	this->FK(&realtime_ik, position_fk);
}

void ArmSolutionBase::SetCurrentPositionAs(FKPosition_XYZRPY* fk_position){
	__current_position_fk.X = fk_position->X;
	__current_position_fk.Y = fk_position->Y;
	__current_position_fk.Z = fk_position->Z;
	__current_position_fk.Roll = fk_position->Roll;
	__current_position_fk.Pitch = fk_position->Pitch;
	__current_position_fk.Yaw = fk_position->Yaw;

	this->IK(&__current_position_fk, &__current_position_ik);
}

void ArmSolutionBase::SetCurrentPositionAs(IKPosition_abgdekl* ik_position){
	for(int i=0; i< CncActuator_List::Instance().GetItemsCount();i++){
		__current_position_ik.Positions[i] = ik_position->Positions[i];
	}
	this->FK(&__current_position_ik, &__current_position_fk);

}
