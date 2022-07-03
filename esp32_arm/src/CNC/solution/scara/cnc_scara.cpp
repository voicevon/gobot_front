#include "cnc_scara.h"

void CncScaraSolution::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);
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
		Serial.println("[Warn] CncScaraSolution::IK()   Please inspect!!!!");
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
		Serial.print("\n[Debug] CncScaraSolution::IK() from (X,Y)=(");
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
void CncScaraSolution::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
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
		Serial.print("\n\n[Debug] CncScaraSolution::FK()  in degree from (alpha,beta) =(");
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

bool CncScaraSolution::GetCurrentPosition(FkPositionBase* position_fk){
	position_fk = & this->__current_fk_position;
	return true;
}

void CncScaraSolution::Init(CncBoardBase* board ){
	Serial.println("[Info] CncScaraSolution::Init() ");
	this->_board = board;
	// this->_scara_machine = (CncSolution_ScaraConfigBase*)(this->_board->GetCncConfig());
	
	board->EnableMotor(AXIS_ALPHA, false);
	board->EnableMotor(AXIS_BETA, false);

	// this->IsInverseKinematicHoimg = true;   // TODO: inside mechanic.

}

float CncScaraSolution::GetDistanceToTarget_FK(){
	// because in this arm solution,  FK is equal to IK. so never mind the logic error.
	// BUT: PLEASE DO NOT REFERENCE THESE CODES!!!
	// TODO: Rewrite this function.
	IkPosition_AB current_ik;
	current_ik.alpha = this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
	current_ik.beta = this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
	FK(&current_ik, &this->__current_fk_position);
	
	float dx = this->__current_fk_position.X - this->__next_fk_position.X;
	float dy = this->__current_fk_position.Y - this->__next_fk_position.Y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

float CncScaraSolution::GetDistanceToTarget_IK(){
	return this->_mover->GetAbsDistanceToTarget_InCncUnit();
}

void CncScaraSolution::RunG1(Gcode* gcode) {
	// Serial.print("\n[Debug] CncScaraSolution::RunG1()   ");
	// Serial.print(gcode->get_command());

	// Assume G1-code mostly wants to update actuator directly, no need to do IK.
	FkPosition_XY target_fk_xy;
	IkPosition_AB target_ik_ab;

	bool do_ik = false;
	uint8_t motor_flags = 0;

	if (gcode->has_letter(AXIS_ALPHA)){
		target_ik_ab.alpha = DEG_TO_RAD * gcode->get_value(AXIS_ALPHA) ;
		motor_flags += 0x01;
	}
	if (gcode->has_letter(AXIS_BETA)) {
		target_ik_ab.beta = DEG_TO_RAD * gcode->get_value(AXIS_BETA);
		motor_flags += 0x02;
	}
	// If need IK, do it now.
	if (gcode->has_letter('X')) {
		do_ik = true;
		target_fk_xy.X = gcode->get_value('X');
		motor_flags = 0x03;
	}
	if (gcode->has_letter('Y')){
		do_ik = true;
		target_fk_xy.Y = gcode->get_value('Y');
		motor_flags = 0x03;
	}
	this->_mover->SetMovingFlags(motor_flags);
	
	if (do_ik) IK(&target_fk_xy, &target_ik_ab);

	bool debug = false;
	if (gcode->has_letter('F')){
		uint speed = gcode->get_value('F');
		this->_mover->SetEefSpeed(DEG_TO_RAD * speed);
		debug = false;
		if (debug){
			Serial.print("[Debug] CncScaraSolution::RunG1()  motor_flags= ");
			Serial.print(motor_flags);
			Serial.print("  speed= ");
			Serial.println(RAD_TO_DEG * speed);
		}
	}
	// TODO:  Enable motor via mover->enable_motor(axis)
	this->_board->EnableMotor(AXIS_ALPHA, true);
	this->_board->EnableMotor(AXIS_BETA, true);

	float cnc_position[2];
	cnc_position[0] = target_ik_ab.alpha;
	cnc_position[1] = target_ik_ab.beta;

	debug = true;
	if (debug){
		Serial.print("\n[Debug] CncScaraSolution::RunG1()  from,to  alpha=");
		Serial.print(RAD_TO_DEG * this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
		Serial.print(" , ");
		Serial.print(RAD_TO_DEG * target_ik_ab.alpha);
		Serial.print("    beta = ");
		Serial.print(RAD_TO_DEG * this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
		Serial.print(" , ");
		Serial.println(RAD_TO_DEG * target_ik_ab.beta);
	}
	uint8_t abs_flags = 0x03;
	this->_mover->AllActuatorsMoveTo(abs_flags, cnc_position);
}



void CncScaraSolution::_SetCurrentPositionAsHome(EnumAxis homing_axis){
	//Set current position to HomePosition
	bool debug = false;
	IkPosition_AB ik_position;
	if (this->_config_base->IsInverseKinematicHoimg){
		if (debug) Serial.print("\n   [Info] CncScaraSolution::_running_G28() Trying to get home position from actuator position  ");
		if (this->_homing_axis_name == AXIS_ALPHA){
			ik_position.alpha =  this->_scara_machine->Homed_position_alpha_in_rad;
			
		}else if (this->_homing_axis_name == AXIS_BETA){
			ik_position.beta =  this->_scara_machine->Homed_position_beta_in_rad;
		}
		this->FK(&ik_position, &this->__current_fk_position);
		// verify FK by IK()
		IkPosition_AB verifying_ik_for_debug;
		// Serial.print("\n\n  [Info] Please verify the below output ======================  ");
		this->IK(&this->__current_fk_position, &verifying_ik_for_debug);
	}
}

// void CncScaraSolution::_running_G28(){
// 	bool debug = false;
// 	if (debug) Serial.print("[Debug] CncScaraSolution::running_G28() is entering \n");
// 	if (this->__homer->IsTriged()){
// 		// End stop is trigered
// 		this->_board->cnc_mover->SingleActuatorStop(this->_homing_axis_name);
// 		//Set current position to HomePosition
// 		IkPosition_AB ik_position;
// 		if (this->_config->IsInverseKinematicHoimg){
// 			if (debug) Serial.print("\n   [Info] CncScaraSolution::_running_G28() Trying to get home position from actuator position  ");
// 			if (this->_homing_axis_name == AXIS_ALPHA){
// 				ik_position.alpha =  this->_scara_machine->Homed_position_alpha_in_rad;
				
// 			}else if (this->_homing_axis_name == AXIS_BETA){
// 				ik_position.beta =  this->_scara_machine->Homed_position_beta_in_rad;
// 			}
// 			this->FK(&ik_position, &this->__current_fk_position);
// 			// verify FK by IK()
// 			IkPosition_AB verifying_ik_for_debug;
// 			// Serial.print("\n\n  [Info] Please verify the below output ======================  ");
// 			this->IK(&this->__current_fk_position, &verifying_ik_for_debug);
// 		}
// 		else{
// 			Serial.print("\n\n\n\n\n  [Error] CncScaraSolution::_running_G28()  Trying to get home position with EEF FK position  ");
// 			while (1){};
// 		}
// 		//Copy current ik-position to motor-position. 
// 		// Note: If homed_position is defined a FK-XY position,  This must be after IK() translation.
// 		if(this->_homing_axis_name == AXIS_ALPHA){
// 			this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_ALPHA, ik_position.alpha);
// 		}else if (this->_homing_axis_name == AXIS_BETA){
// 			this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_BETA, ik_position.beta);
// 		}
// 		this->State = CncState::IDLE;

// 		bool debug = true;
// 		if (debug){
// 			Serial.print("\n[Info] CncScaraSolution::_running_G28() Home sensor is trigger.  " );
// 			Serial.print(this->_homing_axis_name);
// 			if (this->_homing_axis_name ==AXIS_ALPHA){
// 				Serial.print("  cnc position in degree=  ");
// 				Serial.print(RAD_TO_DEG * ik_position.alpha);
// 			}
// 			if (this->_homing_axis_name ==AXIS_BETA){
// 				Serial.print("  cnc position in degree=  ");
// 				Serial.print(RAD_TO_DEG * ik_position.beta);
// 			}
// 		}
// 	}else{
// 		// Endstop is not trigered, When endstop is trigered, must stop the moving. 

// 	}
// }

// void CncScaraSolution::RunM123(uint8_t eef_channel, uint8_t eef_action){
// 	this->_board->GetEef()->Run(eef_action);
// }

// void CncScaraSolution::RunM84(){
// 	this->_board->EnableMotor(AXIS_ALPHA, false);
// 	this->_board->EnableMotor(AXIS_BETA, false);
// }



