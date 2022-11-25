#include "scara_xy_ab_arm_solution.h"

void Scara_ArmSolution::IK(FKPosition_XYZRPY* from_fk, IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPY* fk = (FKPosition_XYZRPY*)(from_fk);
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(to_ik);
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
void Scara_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	IKPosition_abgdekl* ik = from_ik;
	FKPosition_XYZRPY* fk = to_fk;
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

// bool Scara_ArmSolution::GetCurrentPosition(FKPosition_XYZRPY* position_fk){
// 	position_fk = & this->__current_fk_position;
// 	return true;
// }


// float Scara_ArmSolution::GetDistanceToTarget_FK(){
// 	// because in this arm solution,  FK is equal to IK. so never mind the logic error.
// 	// BUT: PLEASE DO NOT REFERENCE THESE CODES!!!
// 	// TODO: Rewrite this function.
// 	IKPosition_abgdekl current_ik;
// 	// current_ik.alpha = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
// 	// current_ik.beta = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
// 	FK(&current_ik, &this->__current_fk_position);
	
// 	float dx = this->__current_fk_position.X - this->__next_fk_position.X;
// 	float dy = this->__current_fk_position.Y - this->__next_fk_position.Y;
// 	float distance = sqrt(dx * dx + dy * dy);
// 	return distance;
// }

// float Scara_ArmSolution::GetDistanceToTarget_IK(){
// 	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
// 	return 0;
// }

// void Scara_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
// 	//Set current position to HomePosition
// 	bool debug = false;
// 	IKPosition_abgdekl ik_position;
// 	// if (this->_config_base.IsInverseKinematicHoimg){
// 		if (debug) Serial.print("\n   [Info] Scara_ArmSolution::_running_G28() Trying to get home position from actuator position  ");
// 		// if (this->_homing_axis == AXIS_ALPHA){
// 		// 	// ik_position.alpha =  this->_scara_machine->Homed_position_alpha_in_rad;
// 		// 	// ik_position.alpha =  this->_scara_machine->GetAxisHomers()->GetAxisHomer(this->_homing_axis)->GetHomingConfig()->LastHomedPosition;
// 		// 	ik_position.alpha =  this->_homer_diction.GetAxisHomer(this->_homing_axis)->GetFiredPosition();
			
// 		// }else if (this->_homing_axis == AXIS_BETA){
// 		// 	// ik_position.beta =  this->_scara_machine->Homed_position_beta_in_rad;
// 		// 	// ik_position.beta =  this->_scara_machine->GetAxisHomers()->GetAxisHomer(this->_homing_axis)->GetHomingConfig()->LastHomedPosition;
// 		// 	ik_position.beta =  this->_homer_diction.GetAxisHomer(this->_homing_axis)->GetFiredPosition();
// 		// }
// 		this->FK(&ik_position, &this->__current_fk_position);
// 		// verify FK by IK()
// 		IKPosition_abgdekl verifying_ik_for_debug;
// 		// Serial.print("\n\n  [Info] Please verify the below output ======================  ");
// 		this->IK(&this->__current_fk_position, &verifying_ik_for_debug);
// 	// }
// }



