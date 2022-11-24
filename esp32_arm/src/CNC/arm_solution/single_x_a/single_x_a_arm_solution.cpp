#include "single_x_a_arm_solution.h"
#include "Robot/axis_homer/home_trigger_array.h"

void SingleAxis_ArmSolution::IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPY* fk = from_fk;
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(to_ik);

	ik->alpha = fk->Roll;
	Logger::Debug("SingleAxis_ArmSolution::IK()");
	Logger::Print("IK output alpha", ik->alpha);
}

void SingleAxis_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	FKPosition_XYZRPY* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	fk->Roll = ik->alpha;
	Logger::Debug("SingleAxis_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->Roll);
}


void SingleAxis_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
	//Set current position to HomePosition
	IKPosition_abgdekl ik_position;
		// We know homed position via FK
		Logger::Info("SingleAxis_ArmSolution::_SetCurrentPositionAsHome()  Trying to get home position with EEF FK position  ");
		// Logger::Print("Config.HomedPosition()", this->_config_base->HomedPosition(AXIS_ALPHA));
		// this->__current_fk_position.A = this->_homer_diction.GetAxisHomer(AXIS_X)->GetFiredPosition();
		this->__current_fk_position.Roll = HomeTrigger_Array::Instance().GetFiredPosition('A');
		this->IK(&this->__current_fk_position, &ik_position);
		// verify IK by FK()
		FKPosition_XYZRPY verifying_fk;
		Serial.print("\n   [Info] Please verify: FK->IK->FK  ");
		this->FK(&ik_position, &verifying_fk);
	//Copy current ik-position to motor-position.
	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
}


// void SingleAxis_ArmSolution::RunG1(Gcode* gcode) {
// bool SingleAxis_ArmSolution::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
// 	Logger::Debug("SingleAxis_ArmSolution::RunG1() is entering");
// 	Logger::Print("G1 ", gcode->get_command());
// 	Logger::Print("this->AXIS", this->_AXIS);
// }

void SingleAxis_ArmSolution::__ConvertSegment_ToMoveBlockQueue(LineSegment* line){
	
}

float SingleAxis_ArmSolution::GetDistanceToTarget_IK(){
	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
	return 1.234;
}


