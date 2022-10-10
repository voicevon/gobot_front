#include "single_x_a_arm_solution.h"


void SingleAxis_ArmSolution::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	FkPosition_A* fk = (FkPosition_A*)(from_fk);
	IkPosition_A* ik = (IkPosition_A*)(to_ik);

	ik->alpha = fk->A;
	Logger::Debug("SingleAxis_ArmSolution::IK()");
	Logger::Print("IK output alpha", ik->alpha);
}

void SingleAxis_ArmSolution::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	FkPosition_A* fk = (FkPosition_A*)(to_fk);
	IkPosition_A* ik = (IkPosition_A*)(from_ik);
	
	fk->A = ik->alpha;
	Logger::Debug("SingleAxis_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->A);
}


void SingleAxis_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
	//Set current position to HomePosition
	IkPosition_A ik_position;
	if (this->_config_base.IsInverseKinematicHoimg){
		// We know homed position via IK.
		Serial.print("\n[Error] SingleAxis_ArmSolution::_running_G28() This robot does NOT impliment this function.");
	}
	else{
		// We know homed position via FK
		Logger::Info("SingleAxis_ArmSolution::_SetCurrentPositionAsHome()  Trying to get home position with EEF FK position  ");
		// Logger::Print("Config.HomedPosition()", this->_config_base->HomedPosition(AXIS_ALPHA));
		// this->__current_fk_position.A = this->_homer_diction.GetAxisHomer(AXIS_X)->GetFiredPosition();
		this->__current_fk_position.A = HomeTrigger_Array::Instance().GetFiredPosition('A');
		this->IK(&this->__current_fk_position, &ik_position);
		// verify IK by FK()
		FkPosition_A verifying_fk;
		Serial.print("\n   [Info] Please verify: FK->IK->FK  ");
		this->FK(&ik_position, &verifying_fk);
	}
	//Copy current ik-position to motor-position.
	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
	// if (this->_homing_axis == this->_AXIS) {
	// 	mb->MoveBlocks[AXIS_ALPHA].axis= AXIS_ALPHA;
	// 	mb->MoveBlocks[AXIS_ALPHA].TargetPosition = ik_position.alpha;
	// 	// this->__queue_move_block->ForwardHead();
	// 	Queue_MoveBlock::Instance().ForwardHead();
	// 	// this->_mover_base->SetActuatorCurrentCncPositionAs(this->_AXIS,ik_position.alpha);
	// }else{
	// 	Logger::Halt("SingleAxis_ArmSolution::_SetCurrentPositionAsHome()");
	// }
}


// void SingleAxis_ArmSolution::RunG1(Gcode* gcode) {
bool SingleAxis_ArmSolution::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
	Logger::Debug("SingleAxis_ArmSolution::RunG1() is entering");
	Logger::Print("G1 ", gcode->get_command());
	Logger::Print("this->AXIS", this->_AXIS);
	// this->_cnc_board->EnableMotor(this->_AXIS, true);
	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
	// MoveBlock* mb = Queue_MoveBlock::Instance().GetHeadMoveblock();
	// float target_speed = 0.1;
	// if (gcode->has_letter('F')){
	// 	target_speed = gcode->get_value('F');
	// 	// this->_mover_base->SetActuatorSpeed(this->_AXIS, target_speed);
	// 	mb->MoveBlocks[AXIS_ALPHA].Speed = target_speed;
	// 	Logger::Print("Target Speed", target_speed);
	// }else{
	// 	//Not set speed. How to get correct default speed?
		
	// }
	// FkPosition_A target_fk_a;
	// IkPosition_A target_ik_a;
	// char axis_name = 'A';
	// bool do_ik = false;
	// if (gcode->has_letter(axis_name)) {
	// 	do_ik = true;
	// 	target_fk_a.A = gcode->get_value(axis_name);
	// 	Logger::Print("G1 target position", target_fk_a.A);
	// }
	// if (do_ik) IK(&target_fk_a, &target_ik_a);

	// //Prepare actuator/driver to move to next point
	// // this->_mover_base->SingleActuatorMoveTo(this->_AXIS, true, target_ik_a.alpha);
	// mb->MoveBlocks[AXIS_ALPHA].IsAbsTargetPosition = true;
	// mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_a.alpha;
	// mb->MoveBlocks[AXIS_ALPHA].Speed = target_speed;
	// // this->_mover_base->SingleActuatorMoveTo(this->_AXIS, true, target_ik_a.alpha);
	// // this->_mover_base->SingleActuatorMoveTo(&move);
	// // this->__queue_move_block->ForwardHead();
	// Queue_MoveBlock::Instance().ForwardHead();

	// //None blocking, move backgroundly.
	// // uint8_t abs_flag=0x01;
	// // this->_mover_base->AllActuatorsMoveTo(abs_flag, &target_ik_a.alpha);
	// bool debug = true;
	// if (debug){
	// 	// this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(this->_AXIS);
	// 	Logger::Debug("SingleAxis_ArmSolution::RunG1() ");
	// 	Logger::Print("from", this->__current_fk_position.A);
	// 	Logger::Print("to", target_ik_a.alpha);
	// }
}

void SingleAxis_ArmSolution::__ConvertSegment_ToMoveBlockQueue(LineSegment* line){
	
}

float SingleAxis_ArmSolution::GetDistanceToTarget_IK(){
	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
	return 1.234;
}


