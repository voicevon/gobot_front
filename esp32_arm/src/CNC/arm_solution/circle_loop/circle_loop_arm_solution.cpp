#include "circle_loop_arm_solution.h"
#include "Robot/axis_homer/home_trigger_array.h"

void CircleLoop_ArmSolution::IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPY* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	// ik->alpha = fk->Roll;
	ik->Actuator[AXIS_ALPHA] = fk->Roll;
	Logger::Debug("CircleLoop_ArmSolution::IK()");
	// Logger::Print("IK output alpha", ik->alpha);
	Logger::Print("IK output alpha", ik->Actuator[AXIS_ALPHA]);
}

void CircleLoop_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	FKPosition_XYZRPY* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	// fk->Roll = ik->alpha;
	fk->Roll = ik->Actuator[AXIS_ALPHA];
	Logger::Debug("CircleLoop_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->Roll);
}


// void CircleLoop_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
// 	//Set current position to HomePosition
// 	IKPosition_abgdekl ik_position;
// 		// We know homed position via FK
// 		Logger::Info("CircleLoop_ArmSolution::_SetCurrentPositionAsHome()  Trying to get home position with EEF FK position  ");
// 		// Logger::Print("Config.HomedPosition()", this->_config_base->HomedPosition(AXIS_ALPHA));
// 		// this->__current_fk_position.A = this->_homer_diction.GetAxisHomer(AXIS_X)->GetFiredPosition();
// 		this->__current_fk_position.Roll = HomeTrigger_Array::Instance().GetFiredPosition('A');
// 		Logger::Print("position trigger, fired position", this->__current_fk_position.Roll);
// 		this->IK(&this->__current_fk_position, &ik_position);
// 		// verify IK by FK()
// 		FKPosition_XYZRPY verifying_fk;
// 		Serial.print("\n   [Info] Please verify: FK->IK->FK  ");
// 		this->FK(&ik_position, &verifying_fk);
// }

// void CircleLoop_ArmSolution::__ConvertSegment_ToMoveBlockQueue(LineSegment* line){
// 	//TODO:  This is a virtual function.
// 	FKPosition_XYZRPY* fk_pos =  &line->TargetPosition;
// 	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
// 	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
// 	mb->MoveBlocks[line->axis].TargetPosition = fk_pos->Roll ;
// 	mb->MoveBlocks[line->axis].Speed = line->Speed;
// 	mb->MoveBlocks[line->axis].Acceleration = line->Acceleration;
// 	Queue_MoveBlock::Instance().Deposit();
// }


// float CircleLoop_ArmSolution::GetDistanceToTarget_IK(){
// 	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
// }


