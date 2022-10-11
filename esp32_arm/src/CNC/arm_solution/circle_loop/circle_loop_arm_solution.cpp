#include "circle_loop_arm_solution.h"
#include "Robot/axis_homer/home_trigger_array.h"

void CircleLoop_ArmSolution::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	FkPosition_A* fk = (FkPosition_A*)(from_fk);
	IkPosition_A* ik = (IkPosition_A*)(to_ik);

	ik->alpha = fk->A;
	Logger::Debug("CircleLoop_ArmSolution::IK()");
	Logger::Print("IK output alpha", ik->alpha);
}

void CircleLoop_ArmSolution::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	FkPosition_A* fk = (FkPosition_A*)(to_fk);
	IkPosition_A* ik = (IkPosition_A*)(from_ik);
	
	fk->A = ik->alpha;
	Logger::Debug("CircleLoop_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->A);
}


void CircleLoop_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
	//Set current position to HomePosition
	IkPosition_A ik_position;
	if (this->_config_base.IsInverseKinematicHoimg){
		// We know homed position via IK.
		Serial.print("\n[Error] CircleLoop_ArmSolution::_running_G28() This robot does NOT impliment this function.");
	}
	else{
		// We know homed position via FK
		Logger::Info("CircleLoop_ArmSolution::_SetCurrentPositionAsHome()  Trying to get home position with EEF FK position  ");
		// Logger::Print("Config.HomedPosition()", this->_config_base->HomedPosition(AXIS_ALPHA));
		// this->__current_fk_position.A = this->_homer_diction.GetAxisHomer(AXIS_X)->GetFiredPosition();
		this->__current_fk_position.A = HomeTrigger_Array::Instance().GetFiredPosition('A');
		Logger::Print("position trigger, fired position", this->__current_fk_position.A);
		this->IK(&this->__current_fk_position, &ik_position);
		// verify IK by FK()
		FkPosition_A verifying_fk;
		Serial.print("\n   [Info] Please verify: FK->IK->FK  ");
		this->FK(&ik_position, &verifying_fk);
	}
	//Copy current ik-position to motor-position.
	// if (this->_homing_axis == this->_AXIS) {
	// 	// this->_mover_base->SetActuatorCurrentCncPositionAs(this->_AXIS,ik_position.alpha);

	// 	// TODO:  todo anything is necessary?
	// }else{
	// 	Logger::Halt("CircleLoop_ArmSolution::_SetCurrentPositionAsHome()");
	// }
}

void CircleLoop_ArmSolution::__ConvertSegment_ToMoveBlockQueue(LineSegment* line){
	//TODO:  This is a virtual function.
	FkPosition_A* fk_pos =  (FkPosition_A*) line->TargetPosition;
	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
	// mb->MoveBlocks[line->axis].axis = line->axis;
	mb->MoveBlocks[line->axis].IsAbsTargetPosition = line->IsAbsTargetPosition;
	mb->MoveBlocks[line->axis].TargetPosition = fk_pos->A ;
	mb->MoveBlocks[line->axis].Speed = line->Speed;
	mb->MoveBlocks[line->axis].Acceleration = line->Acceleration;
	// this->__queue_move_block->ForwardHead();
	Queue_MoveBlock::Instance().Deposit();
}

bool CircleLoop_ArmSolution::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
	Logger::Debug("CircleLoop_ArmSolution::RunG1()");
	Logger::Print("G1 ", gcode->get_command());
	Logger::Print("this->AXIS", this->_AXIS);
	// LineSegment line;

	// // this->_cnc_board->EnableMotor(this->_AXIS, true);
	// if (gcode->has_letter('F')){
	// 	line->Speed = gcode->get_value('F');
	// 	Logger::Print("Speed", line->Speed);
	// }
	// // FkPosition_A target_fk_a;
	// IkPosition_A target_ik_a;
	// char axis_name = 'A';
	// bool do_ik = false;
	// if (gcode->has_letter(axis_name)) {
	// 	do_ik = true;
	// 	this->__target_fk_position.A = gcode->get_value(axis_name);
	// 	Logger::Print("G1 target position", this->__target_fk_position.A);
	// }
	// if (do_ik) IK(&this->__target_fk_position, &target_ik_a);

	// //Prepare actuator/driver to move to next point
	// // this->_mover_base->SingleActuatorMoveTo(this->_AXIS, true, target_ik_a.alpha);
	// line->axis = this->_AXIS;
	// line->IsAbsTargetPosition = true;
	// line->TargetPosition = &this->__target_fk_position;

	// // this->__CutLineSegment_ToMoveBlocks_to_queue(line);

	
	// // this->_mover_base->SingleActuatorMoveTo(&line);   //DOING:  put line to lineSegment queue
	// //None blocking, move backgroundly.
	// // uint8_t abs_flag=0x01;
	// // this->_mover_base->AllActuatorsMoveTo(abs_flag, &target_ik_a.alpha);
	// bool debug = true;
	// if (debug){
	// 	Logger::Print("CircleLoop_ArmSolution::_ConvertG1ToLineSegment() point", 6);
	// 	// this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(this->_AXIS);
	// 	Logger::Print("CircleLoop_ArmSolution::_ConvertG1ToLineSegment() point", 7);

	// 	Logger::Print("from", this->__current_fk_position.A);
	// 	Logger::Print("to", target_ik_a.alpha);
	// }
}


float CircleLoop_ArmSolution::GetDistanceToTarget_IK(){
	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
}


