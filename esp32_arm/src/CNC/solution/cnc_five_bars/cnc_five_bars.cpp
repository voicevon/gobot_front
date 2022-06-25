#include<math.h>
// #include "gobot_chessboard_hw.h"
#include "cnc_five_bars.h"
#include<Arduino.h>

void CncFiveBars::Init(CncBoardBase* board){
	Serial.print("\n[Info] CncFiveBars::Init() is entering.");

	// this->alpha_stepper = board->GetStepper('A');
	// this->beta_stepper = board->GetStepper('B');
	this->alpha_homer = board->GetHomer('A');
	this->beta_homer = board->GetHomer('B');
	this->__eef = board->GetEef();

	board->EnableMotor('A', false);
	board->EnableMotor('B', false);
	this->_board = board;
	this->_fivebarMachine = (CncFiveBarMachine*)(this->_board->GetCncMechanic());
	Serial.print("\n[Info] CncFiveBars::Init() is done.");
} 

void CncFiveBars::RunG28(char axis){ 
	Serial.print("[Debug] CncFiveBars::RunG28() is entering  axis= " );
	Serial.println(axis);
	// this->_homing_axis = axis;
	this->_homing_axis_name = axis;
	float target_position;
	if (axis=='A'){
		this->_board->EnableMotor('A', true);
		// this->alpha_stepper->setAcceleration(this->_fivebarMachine->Homing_acceleration_alpha_beta);
		// this->alpha_stepper->setMaxSpeed(this->_fivebarMachine->Homing_speed_alpha_beta);
		// this->__homing_stepper = this->alpha_stepper;
		this->__current_homer = this->alpha_homer;
		// this->__homing_stepper->setTargetRel(500000);    // angle to be greater.
		target_position = 500000;
	}else if (axis=='B'){
		this->_board->EnableMotor('B', true);
		// int xx =this->_fivebarMachine->Homing_acceleration_alpha_beta;
		int xx =this->_fivebarMachine->HomingAcceleration(AXIS_BETA);
		// this->beta_stepper->setAcceleration(this->_fivebarMachine->Homing_acceleration_alpha_beta);
		// this->beta_stepper->setMaxSpeed(this->_fivebarMachine->Homing_speed_alpha_beta);
		// this->__homing_stepper = this->beta_stepper;
		this->__current_homer = this->beta_homer;
		// this->__homing_stepper->setTargetRel(-500000);    //angle to be smaller.
		target_position = -500000;
	}else{
		Serial.print("\n[Error] CncFiveBars::RunG28() ");
	}
	this->_board->cnc_mover->SingleActuatorMoveTo(axis, false, target_position);
	// this->_stepControl->moveAsync(*this->__homing_stepper);
	Serial.print("[Debug] CncFiveBars::RunG28() is Starting to run...\n" );
}


// When first axis is homed, should it park to somewhere? For the reason, LINK_B might not long enouth to park the second arm.
void CncFiveBars::_running_G28(){
	// Serial.print("[Info] GobotHouseHardware::running_G28() is entering \n");

	if (this->__current_homer->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] CncFiveBars::_running_G28() Home sensor is trigered.  " );
		Serial.print (this->_homing_axis_name);
		// this->_stepControl->stop();
		this->_board->cnc_mover->AllActuatorsStop();

		// The homed postion is a Inverse kinematic position for alpha, beta.
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
			ik_position.alpha =  this->_fivebarMachine->Homed_position_alpha_in_rad;
			ik_position.beta =  this->_fivebarMachine->Homed_position_beta_in_rad;
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik;
			Serial.print("\n\n  [Info] Please verify IK->FK->IK   ");
			this->IK(&this->__current_fk_position, &verifying_ik);
		}else{
			Serial.print("\n  [Error] Trying to get home position with EEF-FK position  ");
		}
		//Copy current ik-position to motor-position.
		// if (this->_homing_axis == 'A') 
		// 	this->alpha_stepper->setPosition(ik_position.alpha * this->_fivebarMachine->STEPS_PER_RAD);
		// if (this->_homing_axis == 'B') 
		// 	this->beta_stepper->setPosition(ik_position.beta * this->_fivebarMachine->STEPS_PER_RAD);
		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs('A', ik_position.alpha);
		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs('B', ik_position.beta);
		
		// this->alpha_stepper->setMaxSpeed(this->_fivebarMachine->MAX_STEPS_PER_SECOND_ALPHA_BETA);
		// this->alpha_stepper->setAcceleration(this->_fivebarMachine->MAX_ACCELERATION_ALPHA_BETA);
		// this->beta_stepper->setMaxSpeed(this->_fivebarMachine->MAX_STEPS_PER_SECOND_ALPHA_BETA);
		// this->beta_stepper->setAcceleration(this->_fivebarMachine->MAX_ACCELERATION_ALPHA_BETA);
		this->State = CncState::IDLE;

	}else{
		// Endstop is not trigered
		// Serial.print("\n[Debug] CncFiveBars::_running_G28()  Still homing\n");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 

	}
}

// https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
void CncFiveBars::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	float rr1= (fk->X + this->_fivebarMachine->LINK_0) * (fk->X + this->_fivebarMachine->LINK_0) + fk->Y * fk->Y;
	// alpha , beta are in unit of RAD.
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf((fk->X + this->_fivebarMachine->LINK_0) / r1);

	float alpha_link = acosf((this->_fivebarMachine->LINK_A * this->_fivebarMachine->LINK_A + rr1 - this->_fivebarMachine->LINK_B * this->_fivebarMachine->LINK_B) / ( this->_fivebarMachine->LINK_A * r1 * 2));
	ik->alpha = alpha_eef + alpha_link;

	bool output_debug = false;
	if (output_debug){
		Serial.print("\nLink0= ");
		Serial.print(this->_fivebarMachine->LINK_0);
		Serial.print(" LinkA= ");
		Serial.print(this->_fivebarMachine->LINK_A);
		Serial.print(" LinkB= ");
		Serial.print(this->_fivebarMachine->LINK_B);
		Serial.print(" r1= ");
		Serial.print(r1);
		Serial.print(" cos (EEF triangle) = ");
		Serial.print((this->_fivebarMachine->LINK_A * this->_fivebarMachine->LINK_A + rr1 - this->_fivebarMachine->LINK_B * this->_fivebarMachine->LINK_B) / ( this->_fivebarMachine->LINK_A * r1 * 2));
		Serial.print(" angle EEF in degree = ");
		Serial.print(alpha_eef * RAD_TO_DEG);
		Serial.print(" angle link in degree = ");
		Serial.print(alpha_link * RAD_TO_DEG);
		Serial.print(" alpha motor angle in degree = ");
		Serial.print(ik->alpha * RAD_TO_DEG);
	}

	float rr2 = (fk->X - this->_fivebarMachine->LINK_0)* (fk->X - this->_fivebarMachine->LINK_0) + fk->Y * fk->Y;
	float r2 = sqrtf(rr2);
	float beta_eef = acosf((fk->X - this->_fivebarMachine->LINK_0) / r2 );
	float beta_link = acosf((this->_fivebarMachine->LINK_A * this->_fivebarMachine->LINK_A + rr2 - this->_fivebarMachine->LINK_B *this->_fivebarMachine-> LINK_B) / (this->_fivebarMachine->LINK_A * r2 * 2));
	ik->beta = beta_eef - beta_link;

	if (output_debug){
		Serial.print("\nLink0= ");
		Serial.print(this->_fivebarMachine->LINK_0);
		Serial.print(" LinkA= ");
		Serial.print(this->_fivebarMachine->LINK_A);
		Serial.print(" LinkB= ");
		Serial.print(this->_fivebarMachine->LINK_B);
		Serial.print(" r2= ");
		Serial.print(r2);
		Serial.print(" cos (EEF triangle) = ");
		Serial.print((this->_fivebarMachine->LINK_A * this->_fivebarMachine->LINK_A + rr2 - this->_fivebarMachine->LINK_B * this->_fivebarMachine->LINK_B) / ( this->_fivebarMachine->LINK_A * r2 * 2));
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

void CncFiveBars::FK(IkPositionBase* from_ik, FkPositionBase* to_fk){
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);

	float elbow_alpha_x = this->_fivebarMachine->LINK_A * cosf(ik->alpha) - this->_fivebarMachine->LINK_0;   // TODO:: whan alpha > 180 degree.
	float elbow_alpha_y = this->_fivebarMachine->LINK_A * sinf(ik->alpha);   // TODO:: When alpha > 90 degree
	float elbow_beta_x = this->_fivebarMachine->LINK_A * cosf(ik->beta) + this->_fivebarMachine->LINK_0;   //TODO: when alpha < 0 degree. 
	float elbow_beta_y = this->_fivebarMachine->LINK_A * sinf(ik->beta);     //TODO: When beta < -90 degree.
	
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
	float lenth_from_center_to_eef = sqrtf(this->_fivebarMachine->LINK_B * this->_fivebarMachine->LINK_B - elbows_distance_sqr / 4);
	if (output_debug){
		Serial.print("\nelbow_distance and lenth_from_center_to_eef = ( ");
		Serial.print(sqrtf(elbows_distance_sqr));
		Serial.print(" , ");
		Serial.print(lenth_from_center_to_eef);
		Serial.print(" )");
	}
	fk->X = center_x + lenth_from_center_to_eef * cosf(rotated_angle);
	fk->Y = center_y + lenth_from_center_to_eef * sinf(rotated_angle);

	Serial.print("\n\n[Debug] CncFiveBars::FK()  in degree from (alpha,beta) =(");
	Serial.print(ik->alpha * RAD_TO_DEG);
	Serial.print(" , ");
	Serial.print(ik->beta * RAD_TO_DEG);
	Serial.print(") \n     Forward Kinematic result:  (X,Y)= (");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}

// void CncFiveBars::RunM123(uint8_t eef_channel, EefAction eef_action){
void CncFiveBars::RunM123(uint8_t eef_channel, uint8_t eef_action){
	Serial.print("[Debug] CncFiveBars::RunM123()  eef_action= ");
	uint8_t action_code = 1;
	this->__eef->Run(action_code);

}

void CncFiveBars::RunG1(Gcode* gcode){
	Serial.print("\n[Debug] CncFiveBars::RunG1()   ");
	Serial.print(gcode->get_command());

	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_XY target_fk_xy;
	IkPosition_AB target_ik_ab;
	// target_fk_xy.X = this->__current_fk_position.X;
	// target_fk_xy.Y = this->__current_fk_position.Y;

	// Sometimes, the current position of stepper is NOT the last target position. Since it's moving.
	// But, The initialized values will effect nothing. They will be over writen. 
	// target_ik_ab.alpha = this->alpha_stepper->getPosition() / this->_fivebarMachine->STEPS_PER_RAD;
	// target_ik_ab.beta = this->beta_stepper->getPosition() / this->_fivebarMachine->STEPS_PER_RAD;
	target_ik_ab.alpha = this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit('A');
	target_ik_ab.beta = this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit('B');
	bool do_ik=false;
	if (gcode->has_letter('A')){
		// this->__EnableMotor('A', true); 
		this->_board->EnableMotor('A', true);
		// target_ik_ab.alpha = gcode->get_value('A') * this->_fivebarMachine->STEPS_PER_RAD * DEG_TO_RAD;
		target_ik_ab.alpha = gcode->get_value('A') * DEG_TO_RAD;
	}
	if (gcode->has_letter('B')){
		// this->__EnableMotor('B', true);
		this->_board->EnableMotor('B', true);
		// target_ik_ab.beta = gcode->get_value('B') * this->_fivebarMachine->STEPS_PER_RAD * DEG_TO_RAD;
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
	// this->alpha_stepper->setTargetAbs(target_ik_ab.alpha * this->_fivebarMachine->STEPS_PER_RAD );
	// this->beta_stepper->setTargetAbs(target_ik_ab.beta * this->_fivebarMachine->STEPS_PER_RAD );
	float target_position[2];
	target_position[0] = target_ik_ab.alpha;
	target_position[1] = target_ik_ab.beta;

	//None blocking, move backgroundly.
	// this->_stepControl->moveAsync(*this->alpha_stepper, *this->beta_stepper);
	this->_board->cnc_mover->AllActuatorsMoveTo(true, target_position);

	if (true){
		FkPosition_XY verified_fk;
		FK(&target_ik_ab, &verified_fk);
		Serial.print("-----------------------------------------------");
		Serial.print("\n Please Verify FK angin to confirm IK() is correct.");
		Serial.print(" FK.X= ");
		Serial.print(verified_fk.X);
		Serial.print(" FK.Y= ");
		Serial.print(verified_fk.Y);

		Serial.print("\n[Debug] CncFiveBars::RunG1() ");
		Serial.print(RAD_TO_DEG * this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit('A'));
		Serial.print(",");
		Serial.print(RAD_TO_DEG * this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit('B'));
		Serial.print(" <-- from   alpha,beta   to --> ");
		Serial.print(RAD_TO_DEG * target_ik_ab.alpha);
		Serial.print(" , ");
		Serial.println(RAD_TO_DEG * target_ik_ab.beta);
	}  
}

void CncFiveBars::_running_G1(){
    // if (this->GetDistanceToTarget_IK() < (this->_fivebarMachine->MAX_STEPS_PER_SECOND_ALPHA_BETA)/32){
    if (this->GetDistanceToTarget_IK() < 123){  // TODO: to determine g1 is finsied

      	this->State = CncState::IDLE;
		Serial.print("\n[Info] CncFiveBars::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);  
}

float CncFiveBars::GetDistanceToTarget_IK(){
	// return this->alpha_stepper->getDistanceToTarget() + this->beta_stepper->getDistanceToTarget();
	return this->_board->cnc_mover->GetAbsDistanceToTarget_InCncUnit();
}

void CncFiveBars::RunM84(){
	// this->__EnableMotor('A', false);
	// this->__EnableMotor('B', false);
	this->_board->EnableMotor('A', false);
	this->_board->EnableMotor('B', false);
}

// void CncFiveBars::__EnableMotor(char actuator, bool enable_it){
// 	if (actuator == 'A')
// 		digitalWrite(PIN_ALPHA_ENABLE, !enable_it);
// 	if (actuator == 'B')
// 		digitalWrite(PIN_BETA_ENABLE, !enable_it);
// }
