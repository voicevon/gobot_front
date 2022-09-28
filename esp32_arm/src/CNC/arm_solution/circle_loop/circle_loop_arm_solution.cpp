#include "circle_loop_arm_solution.h"


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


void CircleLoop_ArmSolution::_SetCurrentPositionAsHome(EnumAxis homing_axis){
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
		this->__current_fk_position.A = this->_cnc_homer.GetAxisHomer(AXIS_ALPHA)->GetFiredPosition();
		Logger::Print("position trigger, fired position", this->__current_fk_position.A);
		this->IK(&this->__current_fk_position, &ik_position);
		// verify IK by FK()
		FkPosition_A verifying_fk;
		Serial.print("\n   [Info] Please verify: FK->IK->FK  ");
		this->FK(&ik_position, &verifying_fk);
	}
	//Copy current ik-position to motor-position.
	if (this->_homing_axis == this->_AXIS) {
		this->_mover_base->SetActuatorCurrentCncPositionAs(this->_AXIS,ik_position.alpha);
	}else{
		Logger::Halt("CircleLoop_ArmSolution::_SetCurrentPositionAsHome()");
	}
}


void CircleLoop_ArmSolution::RunG1(Gcode* gcode) {
	Logger::Debug("CircleLoop_ArmSolution::RunG1() is entering");
	Logger::Print("G1 ", gcode->get_command());
	Logger::Print("this->AXIS", this->_AXIS);
	this->_cnc_board->EnableMotor(this->_AXIS, true);
	if (gcode->has_letter('F')){
		float speed = gcode->get_value('F');
		this->_mover_base->SetActuatorSpeed(this->_AXIS, speed);
		Logger::Print("Speed", speed);
	}
	FkPosition_A target_fk_a;
	IkPosition_A target_ik_a;
	char axis_name = 'A';
	bool do_ik = false;
	if (gcode->has_letter(axis_name)) {
		do_ik = true;
		target_fk_a.A = gcode->get_value(axis_name);
		Logger::Print("G1 target position", target_fk_a.A);
	}
	if (do_ik) IK(&target_fk_a, &target_ik_a);

	//Prepare actuator/driver to move to next point
	// this->_mover_base->SingleActuatorMoveTo(this->_AXIS, true, target_ik_a.alpha);
	LineSegment line;
	line.axis = this->_AXIS;
	line.IsAbsTargetPosition = true;
	line.TargetPosition = target_ik_a.alpha;

	this->__CutLineSegment_ToMoveBlocks_to_queue(&line);

	
	// this->_mover_base->SingleActuatorMoveTo(&line);   //DOING:  put line to lineSegment queue
	//None blocking, move backgroundly.
	// uint8_t abs_flag=0x01;
	// this->_mover_base->AllActuatorsMoveTo(abs_flag, &target_ik_a.alpha);
	bool debug = true;
	if (debug){
		Logger::Print("CircleLoop_ArmSolution::RunG1() point", 6);
		this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(this->_AXIS);
		Logger::Print("CircleLoop_ArmSolution::RunG1() point", 7);

		Logger::Print("from", this->__current_fk_position.A);
		Logger::Print("to", target_ik_a.alpha);
	}
}


float CircleLoop_ArmSolution::GetDistanceToTarget_IK(){
	return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
}


