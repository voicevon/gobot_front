#include<math.h>
#include "five_bars_arm_solution.h"
#include<Arduino.h>

// void FiveBars_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
// 		// The homed postion is a Inverse kinematic position for alpha, beta.
// 		IKPosition_abgdekl ik_position;
// 		// if (this->_config->IsInverseKinematicHoimg){
// 			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
// 			// ik_position.alpha =  this->_config->Homed_position_alpha_in_rad;
// 			// ik_position.beta =  this->_config->Homed_position_beta_in_rad;
// 			// ik_position.alpha = this->_config->GetAxisHomers()->GetAxisHomer(AXIS_ALPHA)->GetHomingConfig()->LastHomedPosition;
// 			// ik_position.beta = this->_config->GetAxisHomers()->GetAxisHomer(AXIS_BETA)->GetHomingConfig()-> LastHomedPosition;
// 			// ik_position.alpha = this->_homer_diction.GetAxisHomer(AXIS_ALPHA)->GetFiredPosition();
// 			// ik_position.beta = this->_homer_diction.GetAxisHomer(AXIS_)->GetFiredPosition();
// 			this->FK(&ik_position, &this->__current_fk_position);
// 			// verify FK by IK()
// 			IKPosition_abgdekl verifying_ik;
// 			Serial.println("\n\n  [Info] Please verify IK->FK->IK   ");
// 			this->IK(&this->__current_fk_position, &verifying_ik);
// }

// https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
void FiveBars_ArmSolution::IK(FKPosition_XYZRPY* from_fk, IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPY* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	float rr1= (fk->X + this->_config->LINK_0) * (fk->X + this->_config->LINK_0) + fk->Y * fk->Y;
	// alpha , beta are in unit of RAD.
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf((fk->X + this->_config->LINK_0) / r1);

	float alpha_link = acosf((this->_config->LINK_A * this->_config->LINK_A + rr1 - this->_config->LINK_B * this->_config->LINK_B) / ( this->_config->LINK_A * r1 * 2));
	// ik->alpha = alpha_eef + alpha_link;
	ik->Actuator[AXIS_ALPHA] = alpha_eef + alpha_link;

	bool output_debug = false;
	if (output_debug){
		Serial.print("\nLink0= ");
		Serial.print(this->_config->LINK_0);
		Serial.print(" LinkA= ");
		Serial.print(this->_config->LINK_A);
		Serial.print(" LinkB= ");
		Serial.print(this->_config->LINK_B);
		Serial.print(" r1= ");
		Serial.print(r1);
		Serial.print(" cos (EEF triangle) = ");
		Serial.print((this->_config->LINK_A * this->_config->LINK_A + rr1 - this->_config->LINK_B * this->_config->LINK_B) / ( this->_config->LINK_A * r1 * 2));
		Serial.print(" angle EEF in degree = ");
		Serial.print(alpha_eef * RAD_TO_DEG);
		Serial.print(" angle link in degree = ");
		Serial.print(alpha_link * RAD_TO_DEG);
		Serial.print(" alpha motor angle in degree = ");
		// Serial.print(ik->alpha * RAD_TO_DEG);
		Serial.print(ik->Actuator[AXIS_ALPHA] * RAD_TO_DEG);
	}

	float rr2 = (fk->X - this->_config->LINK_0)* (fk->X - this->_config->LINK_0) + fk->Y * fk->Y;
	float r2 = sqrtf(rr2);
	float beta_eef = acosf((fk->X - this->_config->LINK_0) / r2 );
	float beta_link = acosf((this->_config->LINK_A * this->_config->LINK_A + rr2 - this->_config->LINK_B *this->_config-> LINK_B) / (this->_config->LINK_A * r2 * 2));
	// ik->beta = beta_eef - beta_link;
	ik->Actuator[AXIS_BETA] = beta_eef - beta_link;

	if (output_debug){
		Serial.print("\nLink0= ");
		Serial.print(this->_config->LINK_0);
		Serial.print(" LinkA= ");
		Serial.print(this->_config->LINK_A);
		Serial.print(" LinkB= ");
		Serial.print(this->_config->LINK_B);
		Serial.print(" r2= ");
		Serial.print(r2);
		Serial.print(" cos (EEF triangle) = ");
		Serial.print((this->_config->LINK_A * this->_config->LINK_A + rr2 - this->_config->LINK_B * this->_config->LINK_B) / ( this->_config->LINK_A * r2 * 2));
		Serial.print(" angle EEF in degree = ");
		Serial.print(beta_eef * RAD_TO_DEG);
		Serial.print(" angle link in degree = ");
		Serial.print(beta_link * RAD_TO_DEG);
		Serial.print(" beta motor angle in degree = ");
		// Serial.print(ik->beta * RAD_TO_DEG);
		Serial.print(ik->Actuator[AXIS_BETA] * RAD_TO_DEG);
	}

  	Logger::Debug("GobotHouseHardware::IK() ");
	Serial.print("from (X,Y)=(");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.println(")");
	Serial.print("kinematic angle degree of origin (ik->alpha, ik->beta)=( ");
	// Serial.print(ik->alpha * RAD_TO_DEG);
	Serial.print(ik->Actuator[AXIS_ALPHA] * RAD_TO_DEG);
	Serial.print(" , ");
	// Serial.print(ik->beta * RAD_TO_DEG);
	Serial.print(ik->Actuator[AXIS_BETA] * RAD_TO_DEG);
	Serial.println(")");
	
}

void FiveBars_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY* to_fk){
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(from_ik);
	FKPosition_XYZRPY * fk = (FKPosition_XYZRPY*)(to_fk);

	// float elbow_alpha_x = this->_config->LINK_A * cosf(ik->alpha) - this->_config->LINK_0;   // TODO:: whan alpha > 180 degree.
	float elbow_alpha_x = this->_config->LINK_A * cosf(ik->Actuator[AXIS_ALPHA]) - this->_config->LINK_0;   // TODO:: whan alpha > 180 degree.
	// float elbow_alpha_y = this->_config->LINK_A * sinf(ik->alpha);   // TODO:: When alpha > 90 degree
	float elbow_alpha_y = this->_config->LINK_A * sinf(ik->Actuator[AXIS_ALPHA]);   // TODO:: When alpha > 90 degree
	// float elbow_beta_x = this->_config->LINK_A * cosf(ik->beta) + this->_config->LINK_0;   //TODO: when alpha < 0 degree. 
	float elbow_beta_x = this->_config->LINK_A * cosf(ik->Actuator[AXIS_BETA]) + this->_config->LINK_0;   //TODO: when alpha < 0 degree. 
	// float elbow_beta_y = this->_config->LINK_A * sinf(ik->beta);     //TODO: When beta < -90 degree.
	float elbow_beta_y = this->_config->LINK_A * sinf(ik->Actuator[AXIS_BETA]);     //TODO: When beta < -90 degree.
	
	bool output_debug = false;
	if (output_debug){
		Serial.print("\nelbow_alpja(x,y) = ( ");
		Serial.print(elbow_alpha_x);
		Serial.print(" , ");
		Serial.print(elbow_alpha_y);
		Serial.print(" )");

		Serial.print("\nelbow_beta(x,y) = ( ");
		Serial.print(elbow_beta_x);
		Serial.print(" , ");
		Serial.print(elbow_beta_y);
		Serial.print(" )");
	}


	float center_x = (elbow_alpha_x + elbow_beta_x) / 2;
	float center_y = (elbow_alpha_y + elbow_beta_y) / 2;
	if (output_debug){
		Serial.print("\nsegment_center(x,y) = ( ");
		Serial.print(center_x);
		Serial.print(" , ");
		Serial.print(center_y);
		Serial.print(" )");
	}
	// float slope = center_y /center_x;
	float delta_x = elbow_beta_x - elbow_alpha_x;
	float delta_y = elbow_beta_y - elbow_alpha_y;
	float origin_slope = delta_y / delta_x;
	float origin_angle = atanf(origin_slope);   // range in degree [-90..+90]
	float rotated_angle = origin_angle + PI / 2; // range in degree [0..180]
	if (output_debug){
		Serial.print("\ndelta(x,y) = ( ");
		Serial.print(delta_x);
		Serial.print(" , ");
		Serial.print(delta_y);
		Serial.print(" )");
		Serial.print("\norigin_slope , orign_angle, rotated_angle  = ( ");
		Serial.print(origin_slope);
		Serial.print(" , ");
		Serial.print(origin_angle * RAD_TO_DEG);	
		Serial.print(" , ");
		Serial.print(rotated_angle * RAD_TO_DEG);
		Serial.print(" )");
	}
	float elbows_distance_sqr = delta_x * delta_x + delta_y * delta_y;
	float lenth_from_center_to_eef = sqrtf(this->_config->LINK_B * this->_config->LINK_B - elbows_distance_sqr / 4);
	if (output_debug){
		Serial.print("\nelbow_distance and lenth_from_center_to_eef = ( ");
		Serial.print(sqrtf(elbows_distance_sqr));
		Serial.print(" , ");
		Serial.print(lenth_from_center_to_eef);
		Serial.print(" )");
	}
	fk->X = center_x + lenth_from_center_to_eef * cosf(rotated_angle);
	fk->Y = center_y + lenth_from_center_to_eef * sinf(rotated_angle);

	Serial.print("\n\n[Debug] FiveBars_ArmSolution::FK()  in degree from (alpha,beta) =(");
	// Serial.print(ik->alpha * RAD_TO_DEG);
	Serial.print(ik->Actuator[AXIS_ALPHA] * RAD_TO_DEG);
	Serial.print(" , ");
	// Serial.print(ik->beta * RAD_TO_DEG);
	Serial.print(ik->Actuator[AXIS_BETA] * RAD_TO_DEG);
	Serial.print(") \n     Forward Kinematic result:  (X,Y)= (");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}

// bool FiveBars_ArmSolution::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
// 	Logger::Warn("FiveBars_ArmSolution::RunG1()");
// 	Serial.println(gcode->get_command());

// 	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
// 	// Assume G1-code want to update actuator directly, no need to do IK.
// 	FKPosition_XYZRPY target_fk_xy;
// 	IKPosition_abgdekl target_ik_ab;

// 	// Sometimes, the current position of stepper is NOT the last target position. Since it's moving.
// 	// But, The initialized values will effect nothing. They will be over writen. 
// 	// target_ik_ab.alpha = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
// 	// target_ik_ab.beta = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
// 	bool do_ik=false;
// 	if (gcode->has_letter('A')){
// 		// this->_cnc_board->EnableMotor(AXIS_ALPHA, true);
// 		target_ik_ab.alpha = gcode->get_value('A') * DEG_TO_RAD;
// 	}
// 	if (gcode->has_letter('B')){
// 		// this->_cnc_board->EnableMotor(AXIS_BETA, true);
// 		target_ik_ab.beta = gcode->get_value('B') *  DEG_TO_RAD;
// 	}
// 	// If need IK, do it now.
// 	if (gcode->has_letter('X')) {
// 		do_ik=true;
// 		target_fk_xy.X = gcode->get_value('X');
// 	}
// 	if (gcode->has_letter('Y')){
// 		do_ik=true;
// 		target_fk_xy.Y = gcode->get_value('Y');
// 	}
// 	if (do_ik) IK(&target_fk_xy,&target_ik_ab);
// 	// Normally the unit in G1A,G1B is degree
// 	if(gcode->has_letter('R')) 
// 		// Bug now, the unit in G1A,G1B is RAD
// 		target_ik_ab.alpha =  gcode->get_value('R');
// 	if (gcode->has_letter('F')){
// 		// float speed = gcode->get_value('F');
// 		// this.sets
// 		// this->beta_stepper->setMaxSpeed(speed);
// 	}
// 	//Prepare actuator/driver to move to next point

// 	//None blocking, move backgroundly.
// 	// this->_mover_base->AllActuatorsMoveTo(true, target_position);
// 	mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_ab.alpha;
// 	mb->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_ab.beta;
// 	// this->__queue_move_block->ForwardHead();
// 	Queue_MoveBlock::Instance().Deposit();

// 	if (true){
// 		FKPosition_XYZRPY verified_fk;
// 		FK(&target_ik_ab, &verified_fk);
// 		Serial.print(" Please Verify FK angin to confirm IK() is correct.");
// 		Serial.print(" FK.X= ");
// 		Serial.print(verified_fk.X);
// 		Serial.print(" FK.Y= ");
// 		Serial.print(verified_fk.Y);

// 		Serial.print("[Debug] FiveBars_ArmSolution::RunG1() ");
// 		// Serial.print(RAD_TO_DEG * this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
// 		Serial.print(",");
// 		// Serial.print(RAD_TO_DEG * this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
// 		Serial.print(" <-- from   alpha,beta   to --> ");
// 		Serial.print(RAD_TO_DEG * target_ik_ab.alpha);
// 		Serial.print(" , ");
// 		Serial.println(RAD_TO_DEG * target_ik_ab.beta);
// 	}  
// }



// float FiveBars_ArmSolution::GetDistanceToTarget_IK(){
// 	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
// }

