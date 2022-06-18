#include "cnc_scara.h"

void CncScara::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);
	bool debug = false;

	float rr1= fk->X * fk->X + fk->Y * fk->Y;

	float beta = PI - acosf((this->_scara_machine->LINK_A * this->_scara_machine->LINK_A + this->_scara_machine->LINK_B * this->_scara_machine->LINK_B -  rr1 ) / (this->_scara_machine->LINK_A * this->_scara_machine->LINK_B * 2));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X / r1);    // [0..PI]
	if (fk->Y < 0)  alpha_eef =  TWO_PI - alpha_eef;  // [0..2*PI]
	float alpha_link = acosf((this->_scara_machine->LINK_A * this->_scara_machine->LINK_A + rr1 - this->_scara_machine->LINK_B * this->_scara_machine->LINK_B)/( this->_scara_machine->LINK_A * r1 * 2));  //[0..PI]
	float alpha = alpha_eef - alpha_link;  //[-PI.. + 2*PI]?

	//TODO: remove this line , it's for Gobot house only.
	if (alpha >  10 * DEG_TO_RAD) alpha -= TWO_PI;   // [-330..+10 ] in degree  
	#define MACHENIC_LIMIT PI * -330 / 180
	if (alpha <  MACHENIC_LIMIT) alpha = MACHENIC_LIMIT ;  // Machnic limitation
	if (debug){
		Serial.print("\n[Debug] CncScara::IK() from (X,Y)=(");
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
		Serial.print(")");
	}
}

// from_ik: Alpha, Beta
//          represents  actuator's current position. unit is rad
// to_fk: x,y.  unit is mm.
void CncScara::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
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
		Serial.print("\n\n[Debug] CncScara::FK()  in degree from (alpha,beta) =(");
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

bool CncScara::GetCurrentPosition(FkPositionBase* position_fk){
	position_fk = & this->__current_fk_position;
	return true;
}

void CncScara::Init(CncBoardBase* board, CncMachineBase* machine){
	Serial.println("[Info] CncScara::Init() ");
	this->_board = board;
	this->_scara_machine = (CncScaraMachine*)(machine);
	
	board->EnableMotor('A', false);
	board->EnableMotor('B', false);

	// this->alpha_stepper->setAcceleration(this->_scara_machine->MAX_ACCELERATION_ALPHPA);
	// this->alpha_stepper->setMaxSpeed(this->_scara_machine->MAX_ACCELERATION_ALPHPA);
	// this->beta_stepper->setAcceleration(this->_scara_machine->MAX_ACCELERATION_BETA);
	// this->beta_stepper->setMaxSpeed(this->_scara_machine->MAX_STEPS_PER_SECOND_BETA);
	// this->alpha_stepper->setInverseRotation(true);
	// this->beta_stepper->setInverseRotation(false);

	this->_home_as_inverse_kinematic = true;
	this->_board->cnc_mover->SetBlockedMove(false);


}

float CncScara::GetDistanceToTarget_FK(){
	// because in this arm solution,  FK is equal to IK. so never mind the logic error.
	// BUT: PLEASE DO NOT REFERENCE THESE CODES!!!
	// TODO: Rewrite this function.
	IkPosition_AB current_ik;
	current_ik.alpha = this->_board->cnc_mover->GetMotorPosition_InCncUnit('A');
	current_ik.beta = this->_board->cnc_mover->GetMotorPosition_InCncUnit('B');
	FK(&current_ik, &this->__current_fk_position);
	
	float dx = this->__current_fk_position.X - this->__next_fk_position.X;
	float dy = this->__current_fk_position.Y - this->__next_fk_position.Y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

float CncScara::GetDistanceToTarget_IK(){
	// int32_t da = this->alpha_stepper->getDistanceToTarget();
	// int32_t db = this->beta_stepper->getDistanceToTarget();
	// float distance = sqrt(da * da + db * db);

	return this->_board->cnc_mover->GetDistanceToTarget_InCncUnit();
}

void CncScara::RunG1(Gcode* gcode) {
	// Serial.print("\n[Debug] CncScara::RunG1()   ");
	// Serial.print(gcode->get_command());
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		// this->alpha_stepper->setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_XY target_fk_xy;
	IkPosition_AB target_ik_ab;
	target_fk_xy.X = this->__current_fk_position.X;
	target_fk_xy.Y = this->__current_fk_position.Y;
	// target_ik_ab.alpha = this->alpha_stepper->getPosition();
	// target_ik_ab.beta = this->beta_stepper->getPosition();
	target_ik_ab.alpha = this->_board->cnc_mover->GetMotorPosition_InCncUnit('A');
	target_ik_ab.beta = this->_board->cnc_mover->GetMotorPosition_InCncUnit('B');
	bool do_ik = false;

	if (gcode->has_letter('A')) 
		// target_ik_ab.alpha = gcode->get_value('A') * this->_scara_machine->STEPS_PER_RAD_ALPHA * DEG_TO_RAD;
		target_ik_ab.alpha = DEG_TO_RAD * gcode->get_value('A') ;
	if (gcode->has_letter('B')) 
		// target_ik_ab.beta = gcode->get_value('B')  * this->_scara_machine->STEPS_PER_RAD_BETA * DEG_TO_RAD;
		target_ik_ab.beta = DEG_TO_RAD * gcode->get_value('B');

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
	if(gcode->has_letter('R')) 
		// target_ik_ab.alpha = this->_scara_machine->motor_steps_per_round * gcode->get_value('R');
		target_ik_ab.alpha = gcode->get_value('R');
	//Prepare actuator/driver to move to next point
	this->_board->EnableMotor('A', true);
	this->_board->EnableMotor('B', true);
	float cnc_position[2];
	cnc_position[0] = target_ik_ab.alpha;
	cnc_position[1] = target_ik_ab.beta;
	//None blocking, move backgroundly.
	this->_board->cnc_mover->AllMotorsMoveTo(true, cnc_position, 2);
	bool debug = true;
	if (debug){
		Serial.print("\n[Debug] CncScara::RunG1()  from,to  alpha=");
		Serial.print(RAD_TO_DEG * this->_board->cnc_mover->GetMotorPosition_InCncUnit('A'));
		Serial.print(" , ");
		Serial.print(RAD_TO_DEG * target_ik_ab.alpha);
		Serial.print("    beta = ");
		Serial.print(RAD_TO_DEG * this->_board->cnc_mover->GetMotorPosition_InCncUnit('B'));
		Serial.print(" , ");
		Serial.println(RAD_TO_DEG * target_ik_ab.beta);
	}
}

void CncScara:: _running_G1(){
    // if (this->GetDistanceToTarget_IK() < (this->_scara_machine->MAX_ACCELERATION_ALPHPA + this->_scara_machine->MAX_ACCELERATION_BETA)/64){
    if (this->GetDistanceToTarget_IK() < 1) {
      	this->State = CncState::IDLE;
		// Serial.print("\n[Info] CncScara::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}

void CncScara::HomeSingleAxis(char axis){
	if (false){
		Serial.print("\n[Debug] CncScara::HomeSingleAxis() is entering   AXIS = " );
		Serial.print(axis);
	}
	this->_homing_axis_name = axis;
	this->__homing_helper = this->_board->GetHomer(axis);
	this->_board->EnableMotor(axis, true);
}

void CncScara::_running_G28(){
	// Serial.print("[Debug] CncScara::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered

		this->_board->cnc_mover->AllMotorStop();

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			// Serial.print("\n   [Info] Trying to get home position from actuator position  ");
			if (this->_homing_axis_name == 'A'){
				ik_position.alpha = DEG_TO_RAD * this->_scara_machine->Homed_position_alpha_in_degree;
				//Copy current ik-position to motor-position.
			}else if (this->_homing_axis_name == 'B'){
				ik_position.beta = DEG_TO_RAD * this->_scara_machine->Homed_position_beta_in_degree;
			}
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik_for_debug;
			// Serial.print("\n\n  [Info] Please verify the below output ======================  ");
			this->IK(&this->__current_fk_position, &verifying_ik_for_debug);
		}
		else{
			Serial.print("\n\n\n\n\n  [Error] CncScara::_running_G28()  Trying to get home position with EEF FK position  ");
		}
		//Copy current ik-position to motor-position. 
		// Note: If homed_position is defined a FK-XY position,  This must be after IK() translation.
		this->_board->cnc_mover->SetMotorPosition('A', ik_position.alpha);
		this->_board->cnc_mover->SetMotorPosition('B', ik_position.beta);
		// Serial.print("\n======================================= End of Homing, State to be IDLE\n");
		this->State = CncState::IDLE;

		bool debug = false;
		if (debug){
			Serial.print("\n[Info] CncScara::_running_G28() Home sensor is trigger.  " );
			Serial.print(this->_homing_axis_name);
			Serial.print("  cnc position degree= ");
			Serial.print(this->_scara_machine->Homed_position_alpha_in_degree);
		}
	}else{
		// Endstop is not trigered, When endstop is trigered, must stop the moving. 
		if (this->_board->cnc_mover->MotorIsMoving(this->_homing_axis_name)){
			return;
		}
		float homing_velocity_in_rad_per_second =  this->_scara_machine->GetHomingVelocity(this->_homing_axis_name);
		this->_board->cnc_mover->SetActuatorSpeed(this->_homing_axis_name, abs(homing_velocity_in_rad_per_second));
		float segment_distance_in_rad = homing_velocity_in_rad_per_second / 10; 
		bool debug = true;
		if(debug){
			Serial.print("[Debug] CncScara::_running_G28() homing_axis= ");
			Serial.print(this->_homing_axis_name);
			Serial.print(" velocity in degree= ");
			Serial.print(RAD_TO_DEG * homing_velocity_in_rad_per_second);
			Serial.print(" current position= ");
			Serial.println(RAD_TO_DEG * this->_board->cnc_mover->GetMotorPosition_InCncUnit(this->_homing_axis_name));
		}
		// this->_board->cnc_mover->SetBlockedMove(true);  //?

		this->_board->cnc_mover->SingleMotorMoveTo(false, this->_homing_axis_name, segment_distance_in_rad);
	}
}

void CncScara::RunM123(uint8_t eef_channel, uint8_t eef_action){
	this->_board->GetEef()->Run(eef_action);
}

void CncScara::RunM84(){
	this->_board->EnableMotor('A', false);
	this->_board->EnableMotor('B', false);
}



