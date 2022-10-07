// #include "box_mover_hw.h"
#include "core_xa_arm_solution.h"




void CncSolution_CoreXA::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] CncSolution_CoreXA::IK() is entering. ");
	FkPosition_ZW* fk = (FkPosition_ZW*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	ik->alpha = (fk->Z  + fk->W);
	ik->beta = (fk->Z  - fk->W );

	Serial.print("\n[Debug] CncSolution_CoreXA::IK() output (alpha, beta) = ");
	Serial.print(ik->alpha);
	Serial.print(" , ");
	Serial.print(ik->beta);
	Serial.print(")");
}

void CncSolution_CoreXA::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] CncSolution_CoreXA::FK() is entering ");
	FkPosition_ZW* fk = (FkPosition_ZW*)(to_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	
	fk->Z = (ik->alpha + ik->beta) / 2 ;
	fk->W = (ik->alpha - ik->beta) / 2 ;

	Serial.print("\n[Debug] CncSolution_CoreXA::FK() output (Z, W) = ");
	Serial.print(fk->Z);
	Serial.print(" , ");
	Serial.print(fk->W);
	Serial.print(")");
}

// void CncSolution_CoreXA::RunG1(Gcode* gcode) {
bool CncSolution_CoreXA::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
	Serial.print("\n[Debug] CncSolution_CoreXA::RunG1() is entering");
	Serial.print(gcode->get_command());
	// this->_cnc_board->EnableMotor(AXIS_ALPHA, true);
	// this->_cnc_board->EnableMotor(AXIS_BETA, true);
	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
	if (gcode->has_letter('F')){
		float speed = gcode->get_value('F');
		// this->_mover_base->SetEefSpeed(speed);
		mb->MoveBlocks[AXIS_ALPHA].Speed = speed;
		mb->MoveBlocks[AXIS_BETA].Speed = speed;
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_ZW target_fk_zw;
	IkPosition_AB target_ik_ab;
	target_fk_zw.Z = this->__current_fk_position.Z;
	target_fk_zw.W = this->__current_fk_position.W;
	// target_ik_ab.alpha = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
	// target_ik_ab.beta = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
	bool do_ik=false;
	if (gcode->has_letter(AXIS_ALPHA)) target_ik_ab.alpha = gcode->get_value(AXIS_ALPHA);
	if (gcode->has_letter(AXIS_BETA)) target_ik_ab.beta = gcode->get_value(AXIS_BETA);

	// If need IK, do it now.
	if (gcode->has_letter('Z')) {
		do_ik=true;
		target_fk_zw.Z = gcode->get_value('Z');
	}
	if (gcode->has_letter('W')){
		do_ik=true;
		target_fk_zw.W = gcode->get_value('W');
	}
	if (do_ik) IK(&target_fk_zw,&target_ik_ab);

	//Prepare actuator/driver to move to next point
	// float motor_position[2];
	// motor_position[0] = target_ik_ab.alpha;
	// motor_position[1] = target_ik_ab.beta;
	mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_ab.alpha;
	mb->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_ab.beta;
	// this->__queue_move_block->ForwardHead();
	Queue_MoveBlock::Instance().Deposit();
	//None blocking, move backgroundly.
	// this->_mover_base->AllActuatorsMoveTo(true, motor_position);

	if (true){
		Serial.print("\n    [Debug] CncSolution_CoreXA::RunG1()     (");
		// Serial.print(this->objStepper_alpha->getPosition());
		// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
		Serial.print(",");
		// Serial.print(this->objStepper_beta->getPosition());
		// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
		Serial.print(")   <-- from   alpha,beta   to -->  (");
		Serial.print(target_ik_ab.alpha  );
		Serial.print(" , ");
		Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}


