#include "scara_xy_ab_arm_solution.h"

void Scara_ArmSolution::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AlphaBeta* ik = (IkPosition_AlphaBeta*)(to_ik);
	bool debug = false;

	float rr1= fk->X * fk->X + fk->Y * fk->Y;
	// beta range:  [0..PI]
	float beta = PI - acosf((this->_scara_machine->LINK_A * this->_scara_machine->LINK_A + this->_scara_machine->LINK_B * this->_scara_machine->LINK_B -  rr1 ) / (this->_scara_machine->LINK_A * this->_scara_machine->LINK_B * 2));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X / r1);    // [0..PI]
	if (fk->Y < 0)  alpha_eef =  TWO_PI - alpha_eef;  // [0..2*PI]
	float alpha_link = acosf((this->_scara_machine->LINK_A * this->_scara_machine->LINK_A + rr1 - this->_scara_machine->LINK_B * this->_scara_machine->LINK_B)/( this->_scara_machine->LINK_A * r1 * 2));  //[0..PI]
	float alpha = alpha_eef - alpha_link;  //[-PI.. + 2*PI]?

	if (alpha < -PI){
		Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
		Serial.println("[Warn] Scara_ArmSolution::IK()   Please inspect!!!!");
		Serial.print("  FK->(X,Y)= ");
		Serial.print(fk->X);
		Serial.print(" , ");
		Serial.print(fk->Y);
		Serial.print("  beta = ");
		Serial.print(RAD_TO_DEG * beta);
		Serial.print(" alpha_eef= ");
		Serial.print(RAD_TO_DEG * alpha_eef);
		Serial.print(" alpha_link= ");
		Serial.print(alpha_link);
		Serial.print(" alpha = ");
		Serial.println(RAD_TO_DEG * alpha);
	}

	//TODO: remove this line , it's for Gobot house only.
	// if (alpha >  10 * DEG_TO_RAD) alpha -= TWO_PI;   // [-330..+10 ] in degree  
	// #define MACHENIC_LIMIT PI * -330 / 180
	// if (alpha <  MACHENIC_LIMIT) alpha = MACHENIC_LIMIT ;  // Machnic limitation
	if (debug){
		Serial.print("\n[Debug] Scara_ArmSolution::IK() from (X,Y)=(");
		Serial.print(fk->X);
		Serial.print(" , ");
		Serial.print(fk->Y);
		Serial.print(")\n    Inverse kinematic angle degree of origin (alpha_eef, alpha_link, ik->alpha)=( ");
		Serial.print(alpha_eef * RAD_TO_DEG);
		Serial.print(" , ");
		Serial.print(alpha_link * RAD_TO_DEG);
		Serial.print(" , ");
		Serial.print(alpha * RAD_TO_DEG);
		Serial.print(")");
	}
	// if (alpha < 0) alpha +=

	ik->alpha = alpha;  // * this->_scara_machine->STEPS_PER_RAD_ALPHA;
	ik->beta =  beta ; // * this->_scara_machine->STEPS_PER_RAD_BETA;

	if (debug){
		Serial.print("\n    Inverse Kinematic result in angle degree (alpha, beta)= ");
		Serial.print(alpha * RAD_TO_DEG);
		Serial.print(" , ");
		Serial.print(beta * RAD_TO_DEG);
		Serial.println(")");
	}
}

// from_ik: Alpha, Beta
//          represents  actuator's current position. unit is rad
// to_fk: x,y.  unit is mm.
void Scara_ArmSolution::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	IkPosition_AlphaBeta* ik = (IkPosition_AlphaBeta*)(from_ik);
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);
	float rad_beta = ik->beta; // / this->_scara_machine->STEPS_PER_RAD_BETA;
	// float rad_eef = rad_beta + ik->alpha / this->_scara_machine->STEPS_PER_RAD_ALPHA;
	float rad_eef = rad_beta + ik->alpha;
	// float rad_alpha = ik->alpha / this->_scara_machine->STEPS_PER_RAD_ALPHA;
	float rad_alpha = ik->alpha; ;
	fk->X = this->_scara_machine->LINK_A * cosf(rad_alpha) + this->_scara_machine->LINK_B * cosf(rad_eef);
	fk->Y = this->_scara_machine->LINK_A * sinf(rad_alpha) + this->_scara_machine->LINK_B * sinf(rad_eef);
	bool debug = false;
	if (debug){
		Serial.print("\n\n[Debug] Scara_ArmSolution::FK()  in degree from (alpha,beta) =(");
		Serial.print(rad_alpha * RAD_TO_DEG);
		Serial.print(" , ");
		Serial.print(rad_beta * RAD_TO_DEG);
		Serial.print(") \n     Forward Kinematic result:  (X,Y)= (");
		Serial.print(fk->X);
		Serial.print(" , ");
		Serial.print(fk->Y);
		Serial.print(")");
	}
}

bool Scara_ArmSolution::GetCurrentPosition(FkPositionBase* position_fk){
	position_fk = & this->__current_fk_position;
	return true;
}


float Scara_ArmSolution::GetDistanceToTarget_FK(){
	// because in this arm solution,  FK is equal to IK. so never mind the logic error.
	// BUT: PLEASE DO NOT REFERENCE THESE CODES!!!
	// TODO: Rewrite this function.
	IkPosition_AlphaBeta current_ik;
	// current_ik.alpha = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
	// current_ik.beta = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
	FK(&current_ik, &this->__current_fk_position);
	
	float dx = this->__current_fk_position.X - this->__next_fk_position.X;
	float dy = this->__current_fk_position.Y - this->__next_fk_position.Y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

float Scara_ArmSolution::GetDistanceToTarget_IK(){
	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
	return 0;
}

// void Scara_ArmSolution::RunG1(Gcode* gcode) {
bool Scara_ArmSolution::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
}

void Scara_ArmSolution::__ConvertSegment_ToMoveBlockQueue(LineSegment* line){
	// Serial.print("\n[Debug] Scara_ArmSolution::RunG1()   ");
	// Serial.print(gcode->get_command());

	// Assume G1-code mostly wants to update actuator directly, no need to do IK.
	FkPosition_XY target_fk_xy;
	IkPosition_AlphaBeta target_ik_ab;
	// Queue_LineSegment* line = this->__queue_move_block->GetHeadMoveblock();
	// Queue_LineSegment* line = Queue_LineSegment::Instance().GetHeadMoveblock();


	bool do_ik = false;
	uint8_t motor_flags = 0;

	// if (gcode->has_letter(AXIS_ALPHA)){
	// 	target_ik_ab.alpha = DEG_TO_RAD * gcode->get_value(AXIS_ALPHA) ;
	// 	motor_flags += 0x01;
	// }
	// if (gcode->has_letter(AXIS_BETA)) {
	// 	target_ik_ab.beta = DEG_TO_RAD * gcode->get_value(AXIS_BETA);
	// 	motor_flags += 0x02;
	// }
	// // If need IK, do it now.
	// if (gcode->has_letter('X')) {
	// 	do_ik = true;
	// 	target_fk_xy.X = gcode->get_value('X');
	// 	motor_flags = 0x03;
	// }
	// if (gcode->has_letter('Y')){
	// 	do_ik = true;
	// 	target_fk_xy.Y = gcode->get_value('Y');
	// 	motor_flags = 0x03;
	// }
	// this->_mover_base->SetMovingFlags(motor_flags);

	
	// if (do_ik) IK(&target_fk_xy, &target_ik_ab);
	// mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_ab.alpha;
	// mb->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_ab.beta;
	
	// bool debug = false;
	// if (gcode->has_letter('F')){
	// 	uint speed = gcode->get_value('F');
	// 	// this->_mover_base->SetEefSpeed(DEG_TO_RAD * speed);
	// 	mb->MoveBlocks[AXIS_ALPHA].Speed = speed;
	// 	mb->MoveBlocks[AXIS_BETA].Speed = speed;
	// 	debug = false;
	// 	if (debug){
	// 		Serial.print("[Debug] Scara_ArmSolution::RunG1()  motor_flags= ");
	// 		Serial.print(motor_flags);
	// 		Serial.print("  speed= ");
	// 		Serial.println(RAD_TO_DEG * speed);
	// 	}
	// }
	// TODO:  Enable motor via mover->enable_motor(axis)
	// this->_cnc_board->EnableMotor(AXIS_ALPHA, true);
	// this->_cnc_board->EnableMotor(AXIS_BETA, true);

	// float cnc_position[2];
	// cnc_position[0] = target_ik_ab.alpha;
	// cnc_position[1] = target_ik_ab.beta;

	// debug = true;
	// if (debug){
	// 	Serial.print("\n[Debug] Scara_ArmSolution::RunG1()  from,to  alpha=");
	// 	// Serial.print(RAD_TO_DEG * this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
	// 	Serial.print(" , ");
	// 	Serial.print(RAD_TO_DEG * target_ik_ab.alpha);
	// 	Serial.print("    beta = ");
	// 	// Serial.print(RAD_TO_DEG * this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
	// 	Serial.print(" , ");
	// 	Serial.println(RAD_TO_DEG * target_ik_ab.beta);
	// }
	// uint8_t abs_flags = 0x03;
	// // this->_mover_base->AllActuatorsMoveTo(abs_flags, cnc_position);
	// this->__queue_move_block->ForwardHead();
}



void Scara_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
	//Set current position to HomePosition
	bool debug = false;
	IkPosition_AlphaBeta ik_position;
	// if (this->_config_base.IsInverseKinematicHoimg){
		if (debug) Serial.print("\n   [Info] Scara_ArmSolution::_running_G28() Trying to get home position from actuator position  ");
		// if (this->_homing_axis == AXIS_ALPHA){
		// 	// ik_position.alpha =  this->_scara_machine->Homed_position_alpha_in_rad;
		// 	// ik_position.alpha =  this->_scara_machine->GetAxisHomers()->GetAxisHomer(this->_homing_axis)->GetHomingConfig()->LastHomedPosition;
		// 	ik_position.alpha =  this->_homer_diction.GetAxisHomer(this->_homing_axis)->GetFiredPosition();
			
		// }else if (this->_homing_axis == AXIS_BETA){
		// 	// ik_position.beta =  this->_scara_machine->Homed_position_beta_in_rad;
		// 	// ik_position.beta =  this->_scara_machine->GetAxisHomers()->GetAxisHomer(this->_homing_axis)->GetHomingConfig()->LastHomedPosition;
		// 	ik_position.beta =  this->_homer_diction.GetAxisHomer(this->_homing_axis)->GetFiredPosition();
		// }
		this->FK(&ik_position, &this->__current_fk_position);
		// verify FK by IK()
		IkPosition_AlphaBeta verifying_ik_for_debug;
		// Serial.print("\n\n  [Info] Please verify the below output ======================  ");
		this->IK(&this->__current_fk_position, &verifying_ik_for_debug);
	// }
}



