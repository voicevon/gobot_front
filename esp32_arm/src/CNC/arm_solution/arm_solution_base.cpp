#include "arm_solution_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
// #include "MyLibs/basic/queue/message_queue.h"
// #include "Mqtt/mqtt_message_queue.h"

void ArmSolutionBase::ForceStopMover(){
	// this->_mover_base->AllActuatorsStop();

	//TODO:    How to do it without mover_base??
}

void ArmSolutionBase::GetRealTimePosition(FKPosition_XYZRPW* position_fk){
	//TODO::  
	IKPosition_abgdekl realtime_ik;
	// realtime_ik.alpha = gs_CncActuator_List::Instance().GetActuator(AXIS_ALPHA)->GetCurrentPosition();
	realtime_ik.Positions[AXIS_ALPHA] = gs_CncActuator_List::Instance().GetActuator(AXIS_ALPHA)->GetCurrentPosition();
	this->IK_to_FK(&realtime_ik, position_fk);
}

void ArmSolutionBase::SetCurrentPositionAs(FKPosition_XYZRPW* fk_position){
	__current_position_fk.X = fk_position->X;
	__current_position_fk.Y = fk_position->Y;
	__current_position_fk.Z = fk_position->Z;
	__current_position_fk.Roll = fk_position->Roll;
	__current_position_fk.Pitch = fk_position->Pitch;
	__current_position_fk.Yaw = fk_position->Yaw;

	// this->IK(&__current_position_fk, &__current_position_ik);
	// __current_position_fk.PrintOut("Caller = ArmSolutionBase::SetCurrentPositionAs()  homed positions FK");
}

void ArmSolutionBase::SetCurrentPositionAs(IKPosition_abgdekl* ik_position){
	for(int i=0; i< gs_CncActuator_List::Instance().GetItemsCount();i++){
		__current_position_ik.Positions[i] = ik_position->Positions[i];
		CncActuatorBase* actuator = gs_CncActuator_List::Instance().GetActuator(i);
		actuator->SetCurrentPositionAs(__current_position_ik.Positions[i]);
	__current_position_ik.Positions[i] = ik_position->Positions[i];
	}
	// this->FK(&__current_position_ik, &__current_position_fk);
	// __current_position_ik.PrintOut("Caller = ArmSolutionBase::SetCurrentPositionAs()  homed positions IK");
}


void ArmSolutionBase::Test(FKPosition_XYZRPW* input){
	IKPosition_abgdekl ik;
	// FKPosition_XYZRPW input;
	FKPosition_XYZRPW output;
	this->FK_to_IK(input,&ik);
	this->IK_to_FK(&ik,&output);
	Logger::Info("Twh_ArmSolution::test()");
	Serial.print("input \t");
	Serial.print(input->X);
	Serial.print("\t");
	Serial.println(input->Y);
	Serial.print("\t");
	Serial.println(input->Z);

	Serial.print("output \t");
	Serial.print(output.X);
	Serial.print("\t");
	Serial.println(output.Y);
	Serial.print("\t");
	Serial.println(output.Z);

}