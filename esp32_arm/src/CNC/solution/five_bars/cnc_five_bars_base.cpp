#include<math.h>
#include "cnc_five_bars_base.h"
#include<Arduino.h>

void CncSolution_FiveBarsBase::Init(CncBoardBase* board){
	Serial.println("[Info] CncSolution_FiveBarsBase::Init() is entering.");
	this->LinkEef(board->GetEef());


	board->EnableMotor(AXIS_ALPHA, false);
	board->EnableMotor(AXIS_BETA, false);
	this->_board = board;
	this->_config = (CncSolution_FiveBarConfig*)(this->_board->GetCncConfig());
	Serial.println("[Info] CncSolution_FiveBarsBase::Init() is done.");
} 

void CncSolution_FiveBarsBase::_SetCurrentPositionAsHome(EnumAxis homing_axis){
		// The homed postion is a Inverse kinematic position for alpha, beta.
		IkPosition_AB ik_position;
		Serial.print("222222222222222222");
		this->_config->PrintOut("ggggggggggggggggggggggggggggggggggggggggggggggggggg");
		if (this->_config->IsInverseKinematicHoimg){
			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
			ik_position.alpha =  this->_config->Homed_position_alpha_in_rad;
			ik_position.beta =  this->_config->Homed_position_beta_in_rad;
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik;
			Serial.print("\n\n  [Info] Please verify IK->FK->IK   ");
			this->IK(&this->__current_fk_position, &verifying_ik);
		}else{
			Logger::Error("CncSolution_FiveBarsBase::_running_G28()  Trying to get home position");
			Serial.print(" with EEF-FK position is under construction");
			Serial.println(FCBC_RESET);
		}
		//Copy current ik-position to motor-position.
		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_ALPHA, ik_position.alpha);
		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_BETA, ik_position.beta);
}

// void CncSolution_FiveBarsBase::RunG28(EnumAxis axis){ 
// 	Serial.print("[Debug] CncSolution_FiveBarsBase::RunG28() is entering  axis= " );
// 	Serial.println(axis);
// 	// this->_homing_axis = axis;
// 	if ( axis==AXIS_ALPHA || axis == AXIS_BETA){
// 		this->_homing_axis_name = axis;
// 		this->__current_homer = this->_board->GetHomer(axis);
		
// 		CncSolutionConfigBase* config = this->_board->GetCncMechanic();
// 		CncMoverBase* mover = this->_board->cnc_mover;
// 		config->PrintOut("Config in CncSolution_FiveBarsBase::RunG28()");
// 		mover->SetActuatorSpeed(axis, config->HomingSpeed(axis));
// 		// mover.setAcceleration(axis, config->HomingAcceleration(axis);
// 		this->_board->EnableMotor(axis, true);
// 		float long_distance_to_move = 999.0f * config->HomingDir_IsToMax(axis);

// 		mover->SingleActuatorMoveTo(axis, false, long_distance_to_move);
// 		mover->PrintOut("Mover in CncSolution_FiveBarsBase::RunG28()");
// 	}else{
// 		Logger::Error("CncSolution_FiveBarsBase::RunG28() ");
// 	}
// 	// this->_stepControl->moveAsync(*this->__homing_stepper);
// 	Serial.println("[Debug] CncSolution_FiveBarsBase::RunG28() is Starting to run..." );
// }


// When first axis is homed, should it park to somewhere? For the reason, LINK_B might not long enouth to park the second arm.

// When first axis is homed, should it park to somewhere? For the reason, LINK_B might not long enouth to park the second arm.
// void CncSolution_FiveBarsBase::_running_G28(){
// 	// Serial.print("[Info] GobotHouseHardware::running_G28() is entering \n");

// 	if (this->__current_homer->IsTriged()){
// 		// End stop is trigered
// 		Serial.print("[Info] CncSolution_FiveBarsBase::_running_G28() Home sensor is trigered.  axis= " );
// 		Serial.println (this->_homing_axis_name);
// 		this->_board->RepportRamUsage();

// 		this->_board->cnc_mover->AllActuatorsStop();
// 		// The homed postion is a Inverse kinematic position for alpha, beta.
// 		IkPosition_AB ik_position;
// 		Serial.print("222222222222222222");
// 		this->_config->PrintOut("ggggggggggggggggggggggggggggggggggggggggggggggggggg");
// 		if (this->_config->IsInverseKinematicHoimg){
// 			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
// 			ik_position.alpha =  this->_config->Homed_position_alpha_in_rad;
// 			ik_position.beta =  this->_config->Homed_position_beta_in_rad;
// 			this->FK(&ik_position, &this->__current_fk_position);
// 			// verify FK by IK()
// 			IkPosition_AB verifying_ik;
// 			Serial.print("\n\n  [Info] Please verify IK->FK->IK   ");
// 			this->IK(&this->__current_fk_position, &verifying_ik);
// 		}else{
// 			Logger::Error("CncSolution_FiveBarsBase::_running_G28()  Trying to get home position");
// 			Serial.print(" with EEF-FK position is under construction");
// 			Serial.println(FCBC_RESET);
// 		}
// 		//Copy current ik-position to motor-position.
// 		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_ALPHA, ik_position.alpha);
// 		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_BETA, ik_position.beta);
		
// 		this->State = CncState::IDLE;

// 	}else{
// 		// Endstop is not trigered

// 	}
// }
// https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
void CncSolution_FiveBarsBase::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	float rr1= (fk->X + this->_config->LINK_0) * (fk->X + this->_config->LINK_0) + fk->Y * fk->Y;
	// alpha , beta are in unit of RAD.
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf((fk->X + this->_config->LINK_0) / r1);

	float alpha_link = acosf((this->_config->LINK_A * this->_config->LINK_A + rr1 - this->_config->LINK_B * this->_config->LINK_B) / ( this->_config->LINK_A * r1 * 2));
	ik->alpha = alpha_eef + alpha_link;

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
		Serial.print(ik->alpha * RAD_TO_DEG);
	}

	float rr2 = (fk->X - this->_config->LINK_0)* (fk->X - this->_config->LINK_0) + fk->Y * fk->Y;
	float r2 = sqrtf(rr2);
	float beta_eef = acosf((fk->X - this->_config->LINK_0) / r2 );
	float beta_link = acosf((this->_config->LINK_A * this->_config->LINK_A + rr2 - this->_config->LINK_B *this->_config-> LINK_B) / (this->_config->LINK_A * r2 * 2));
	ik->beta = beta_eef - beta_link;

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
		Serial.print(ik->beta * RAD_TO_DEG);
	}

  	Serial.print("\n[Debug] GobotHouseHardware::IK() from (X,Y)=(");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")\n    Inverse kinematic angle degree of origin (ik->alpha, ik->beta)=( ");
	Serial.print(ik->alpha * RAD_TO_DEG);
	Serial.print(" , ");
	Serial.print(ik->beta * RAD_TO_DEG);
	Serial.print(")");
}

void CncSolution_FiveBarsBase::FK(IkPositionBase* from_ik, FkPositionBase* to_fk){
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);

	float elbow_alpha_x = this->_config->LINK_A * cosf(ik->alpha) - this->_config->LINK_0;   // TODO:: whan alpha > 180 degree.
	float elbow_alpha_y = this->_config->LINK_A * sinf(ik->alpha);   // TODO:: When alpha > 90 degree
	float elbow_beta_x = this->_config->LINK_A * cosf(ik->beta) + this->_config->LINK_0;   //TODO: when alpha < 0 degree. 
	float elbow_beta_y = this->_config->LINK_A * sinf(ik->beta);     //TODO: When beta < -90 degree.
	
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

	Serial.print("\n\n[Debug] CncSolution_FiveBarsBase::FK()  in degree from (alpha,beta) =(");
	Serial.print(ik->alpha * RAD_TO_DEG);
	Serial.print(" , ");
	Serial.print(ik->beta * RAD_TO_DEG);
	Serial.print(") \n     Forward Kinematic result:  (X,Y)= (");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}

// void CncSolution_FiveBarsBase::RunM123(uint8_t eef_channel, EefAction eef_action){

void CncSolution_FiveBarsBase::RunG1(Gcode* gcode){
	Serial.print("[Debug] CncSolution_FiveBarsBase::RunG1()   ");
	Serial.println(gcode->get_command());

	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_XY target_fk_xy;
	IkPosition_AB target_ik_ab;

	// Sometimes, the current position of stepper is NOT the last target position. Since it's moving.
	// But, The initialized values will effect nothing. They will be over writen. 
	target_ik_ab.alpha = this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
	target_ik_ab.beta = this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
	bool do_ik=false;
	if (gcode->has_letter('A')){
		this->_board->EnableMotor(AXIS_ALPHA, true);
		target_ik_ab.alpha = gcode->get_value('A') * DEG_TO_RAD;
	}
	if (gcode->has_letter('B')){
		this->_board->EnableMotor(AXIS_BETA, true);
		target_ik_ab.beta = gcode->get_value('B') *  DEG_TO_RAD;
	}
	// If need IK, do it now.
	if (gcode->has_letter('X')) {
		do_ik=true;
		target_fk_xy.X = gcode->get_value('X');
	}
	if (gcode->has_letter('Y')){
		do_ik=true;
		target_fk_xy.Y = gcode->get_value('Y');
	}
	if (do_ik) IK(&target_fk_xy,&target_ik_ab);
	// Normally the unit in G1A,G1B is degree
	if(gcode->has_letter('R')) 
		// Bug now, the unit in G1A,G1B is RAD
		target_ik_ab.alpha =  gcode->get_value('R');
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		// this->beta_stepper->setMaxSpeed(speed);
	}
	//Prepare actuator/driver to move to next point
	float target_position[2];
	target_position[0] = target_ik_ab.alpha;
	target_position[1] = target_ik_ab.beta;

	//None blocking, move backgroundly.
	this->_board->cnc_mover->AllActuatorsMoveTo(true, target_position);

	if (true){
		FkPosition_XY verified_fk;
		FK(&target_ik_ab, &verified_fk);
		Serial.println("-----------------------------------------------");
		Serial.print(" Please Verify FK angin to confirm IK() is correct.");
		Serial.print(" FK.X= ");
		Serial.print(verified_fk.X);
		Serial.print(" FK.Y= ");
		Serial.print(verified_fk.Y);

		Serial.print("[Debug] CncSolution_FiveBarsBase::RunG1() ");
		Serial.print(RAD_TO_DEG * this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
		Serial.print(",");
		Serial.print(RAD_TO_DEG * this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
		Serial.print(" <-- from   alpha,beta   to --> ");
		Serial.print(RAD_TO_DEG * target_ik_ab.alpha);
		Serial.print(" , ");
		Serial.println(RAD_TO_DEG * target_ik_ab.beta);
	}  
}



float CncSolution_FiveBarsBase::GetDistanceToTarget_IK(){
	return this->_board->cnc_mover->GetAbsDistanceToTarget_InCncUnit();
}

// void CncSolution_FiveBarsBase::RunM84(){
// 	this->_board->EnableMotor(AXIS_ALPHA, false);
// 	this->_board->EnableMotor(AXIS_BETA, false);
// }

