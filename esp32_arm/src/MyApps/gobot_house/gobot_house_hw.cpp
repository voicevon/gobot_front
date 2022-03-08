#include "gobot_house_hw.h"


// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/

// }
void GobotHouseHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);
	// bool beta_reverse = false;
	// if (fk->X <0 && fk->Y >0) {
	// 	fk->Y = 0- fk->Y;
	// 	beta_reverse = true;
	// }
	float rr1= fk->X * fk->X + fk->Y * fk->Y;

	float beta = PI - acosf((this->__config.LINK_A * this->__config.LINK_A + this->__config.LINK_B * this->__config.LINK_B -  rr1 ) / (this->__config.LINK_A * this->__config.LINK_B * 2));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X / r1);    // [0..PI]
	if (fk->Y < 0)  alpha_eef =  TWO_PI - alpha_eef;  // [0..2*PI]
	float alpha_link = acosf((this->__config.LINK_A * this->__config.LINK_A + rr1 - this->__config.LINK_B * this->__config.LINK_B)/( this->__config.LINK_A * r1 * 2));  //[0..PI]
	float alpha = alpha_eef - alpha_link;  //[-PI.. + 2*PI]?
	if (alpha >  10 * DEG_TO_RAD) alpha -= TWO_PI;   // [-330..+10 ] in degree
	// if (beta_reverse){
	// 	beta = 0.0 - beta;
	// 	float d_alpha =  PI * 3 - alpha;
	// }
	#define MACHENIC_LIMIT PI * -330 / 180
	if (alpha <  MACHENIC_LIMIT) alpha = MACHENIC_LIMIT ;  // Machnic limitation
	if (false){
		Serial.print("\n[Debug] GobotHouseHardware::IK() from (X,Y)=(");
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

	ik->alpha = alpha * this->__config.STEPS_PER_RAD_ALPHA;
	ik->beta =  beta * this->__config.STEPS_PER_RAD_BETA;

	if (false){
		Serial.print("\n    Inverse Kinematic result in angle degree (alpha, beta)= ");
		Serial.print(alpha * RAD_TO_DEG);
		Serial.print(" , ");
		Serial.print(beta * RAD_TO_DEG);
		Serial.print(")");
	}
}

// from_ik: Alpha, Beta
//          represents  actuator's current position. unit is step
// to_fk: x,y.  unit is mm.
void GobotHouseHardware::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);
	float rad_beta = ik->beta / this->__config.STEPS_PER_RAD_BETA;
	float rad_eef = rad_beta + ik->alpha / this->__config.STEPS_PER_RAD_ALPHA;
	float rad_alpha = ik->alpha / this->__config.STEPS_PER_RAD_ALPHA;
	fk->X = this->__config.LINK_A * cosf(rad_alpha) + this->__config.LINK_B * cosf(rad_eef);
	fk->Y = this->__config.LINK_A * sinf(rad_alpha) + this->__config.LINK_B * sinf(rad_eef);
	if (false){
		Serial.print("\n\n[Debug] GobotHouseHardware::FK()  in degree from (alpha,beta) =(");
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

GobotHouseHardware::GobotHouseHardware(){
	this->__config.Init();
}



bool GobotHouseHardware::GetCurrentPosition(FkPositionBase* position_fk){
	position_fk = & this->__current_fk_position;
	return true;
}


void GobotHouseHardware::__Init_gpio(){
	pinMode(PIN_ALPHA_ENABLE, OUTPUT);
	pinMode(PIN_BETA_ENABLE, OUTPUT);

	// TODO:  shoud do this? double check document
	// pinMode(PIN_ENDER_COIL_2109, OUTPUT);
	// pinMode(PIN_ENDER_COIL_EXT_2109, OUTPUT);
	
	// digitalWrite(PIN_ENDER_COIL_2109, LOW);
	// digitalWrite(PIN_ENDER_COIL_EXT_2109, LOW);
	  // configure LED PWM functionalitites
	ledcSetup(1, 200, 8);  //ledcSetup(ledChannel, freq, resolution);
	ledcSetup(2, 200, 8);  //ledcSetup(ledChannel, freq, resolution);
	// attach the channel to the GPIO to be controlled
	ledcAttachPin(PIN_ENDER_COIL_2109, 1); // ledcAttachPin(ledPin, ledChannel);
	ledcAttachPin(PIN_ENDER_COIL_EXT_2109, 2); // ledcAttachPin(ledPin, ledChannel);
	ledcWrite(1, 0);
	ledcWrite(2, 0);
}
void GobotHouseHardware::InitRobot(){
	__Init_gpio();
	this->__EnableMotor('A', false);
	this->__EnableMotor('B', false);

	CommuUart* commuUart = new CommuUart();

	this->commuDevice = commuUart; 
	this->objStepper_alpha.setAcceleration(this->__config.MAX_ACCELERATION_ALPHPA);
	this->objStepper_alpha.setMaxSpeed(this->__config.MAX_ACCELERATION_ALPHPA);
	this->objStepper_beta.setAcceleration(this->__config.MAX_ACCELERATION_BETA);
	this->objStepper_beta.setMaxSpeed(this->__config.MAX_STEPS_PER_SECOND_BETA);
	this->objStepper_alpha.setInverseRotation(true);
	this->objStepper_beta.setInverseRotation(false);

	this->_home_as_inverse_kinematic = true;

}

float GobotHouseHardware::GetDistanceToTarget_FK(){
	// because in this arm solution,  FK is equal to IK. so never mind the logic error.
	// BUT: PLEASE DO NOT REFERENCE THESE CODES!!!
	// TODO: Rewrite this function.
	IkPosition_AB current_ik;
	current_ik.alpha = (float)this->objStepper_alpha.getPosition();
	current_ik.beta = (float)this->objStepper_beta.getPosition();
	FK(&current_ik, &this->__current_fk_position);
	
	float dx = this->__current_fk_position.X - this->__next_fk_position.X;
	float dy = this->__current_fk_position.Y - this->__next_fk_position.Y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

float GobotHouseHardware::GetDistanceToTarget_IK(){
	int32_t da = this->objStepper_alpha.getDistanceToTarget();
	int32_t db = this->objStepper_beta.getDistanceToTarget();
	float distance = sqrt(da * da + db * db);
	return distance;
}

void GobotHouseHardware::RunG1(Gcode* gcode) {
	// Serial.print("\n[Debug] GobotHouseHardware::RunG1()   ");
	// Serial.print(gcode->get_command());
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_XY target_fk_xy;
	IkPosition_AB target_ik_ab;
	target_fk_xy.X = this->__current_fk_position.X;
	target_fk_xy.Y = this->__current_fk_position.Y;
	target_ik_ab.alpha = this->objStepper_alpha.getPosition();
	target_ik_ab.beta = this->objStepper_beta.getPosition();
	bool do_ik=false;

	if (gcode->has_letter('A')) 
		target_ik_ab.alpha = gcode->get_value('A') * this->__config.STEPS_PER_RAD_ALPHA * DEG_TO_RAD;
	if (gcode->has_letter('B')) 
		target_ik_ab.beta = gcode->get_value('B') * this->__config.STEPS_PER_RAD_BETA * DEG_TO_RAD;

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
	if(gcode->has_letter('R')) target_ik_ab.alpha = this->__config.motor_steps_per_round * gcode->get_value('R');
	//Prepare actuator/driver to move to next point
	this->__EnableMotor('A', true);
	this->__EnableMotor('B',true);
	this->objStepper_alpha.setTargetAbs(target_ik_ab.alpha );
	this->objStepper_beta.setTargetAbs(target_ik_ab.beta);
	//None blocking, move backgroundly.
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);

	if (false){
		Serial.print("\n[Debug] GobotHouseHardware::RunG1() ");
		Serial.print(this->objStepper_alpha.getPosition());
		Serial.print(",");
		Serial.print(this->objStepper_beta.getPosition());
		Serial.print(" <-- from   alpha,beta   to --> ");
		Serial.print(target_ik_ab.alpha);
		Serial.print(">>");
		Serial.print(target_ik_ab.alpha);
		Serial.print(" , ");
		Serial.println(target_ik_ab.beta);
	}
}
void GobotHouseHardware:: _running_G1(){
    if (this->GetDistanceToTarget_IK() < (this->__config.MAX_ACCELERATION_ALPHPA + this->__config.MAX_ACCELERATION_BETA)/64){
      	this->State = RobotState::IDLE;
		// Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
void GobotHouseHardware::HomeSingleAxis(char axis){
	if (false){
		Serial.print("\n[Debug] GobotHouseHardware::HomeSingleAxis() is entering   AXIS = " );
		Serial.print(axis);
	}
	this->_homing_axis = axis;
	this->__EnableMotor(axis, true);
	if (axis=='A'){
		this->objStepper_alpha.setAcceleration(this->__config.Homing_acceleration_alpha);
		this->objStepper_alpha.setMaxSpeed(this->__config.Homing_speed_alpha);
		this->__homing_stepper = &this->objStepper_alpha;
		this->__homing_helper = &this->objHomeHelper_alpha;
	}else if (axis=='B'){
		this->objStepper_beta.setAcceleration(this->__config.Homing_acceleration_beta);
		this->objStepper_beta.setMaxSpeed(this->__config.Homing_speed_beta);
		this->__homing_stepper = &this->objStepper_beta;
		this->__homing_helper = &this->objHomeHelper_beta;
	}
	this->__homing_stepper->setTargetRel(500000);
	this->objStepControl.moveAsync(*this->__homing_stepper);
}

void GobotHouseHardware::_running_G28(){
	// Serial.print("[Debug] GobotHouseHardware::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		if (false){
			Serial.print("\n[Info] GobotHouseHardware::_running_G28() Home sensor is trigger.  " );
			Serial.print (this->_homing_axis);
		}
		this->objStepControl.stop();

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			// Serial.print("\n   [Info] Trying to get home position from actuator position  ");

			ik_position.alpha = DEG_TO_RAD * this->__config.Homed_position_alpha_in_degree * this->__config.STEPS_PER_RAD_ALPHA;
			ik_position.beta =  DEG_TO_RAD * this->__config.Homed_position_beta_in_degree * this->__config.STEPS_PER_RAD_BETA;
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik_for_debug;
			// Serial.print("\n\n  [Info] Please verify the below output ======================  ");
			this->IK(&this->__current_fk_position, &verifying_ik_for_debug);
		}
		else{
			Serial.print("\n\n\n\n\n  [Error] Trying to get home position with EEF position  ");
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == 'A') this->objStepper_alpha.setPosition(ik_position.alpha);
		if (this->_homing_axis == 'B') this->objStepper_beta.setPosition(ik_position.beta);
		
		this->objStepper_alpha.setMaxSpeed(this->__config.MAX_STEPS_PER_SECOND_ALPHA);
		this->objStepper_alpha.setAcceleration(this->__config.MAX_ACCELERATION_ALPHPA);
		this->objStepper_beta.setMaxSpeed(this->__config.MAX_STEPS_PER_SECOND_BETA);
		this->objStepper_beta.setAcceleration(this->__config.MAX_ACCELERATION_BETA);
		// Serial.print("\n======================================= End of Homing, State to be IDLE\n");
		this->State = RobotState::IDLE;

	}else{
		// Endstop is not trigered
		// Serial.print("[Debug] Still homing\n");
		// Serial.print("<");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 
		// this->__homing_stepper->setTargetRel(50000);
		// this->objStepControl.moveAsync(*this->__homing_stepper);
	}
}

// We want to fix pin_number in firmware, 
// So eef_channel takes no effection.
void GobotHouseHardware::RunM123(uint8_t eef_channel, EefAction eef_action){
	switch (eef_action){

		case EefAction::Suck:
			// Serial.print("\nGobotHouseHardware::RunM123()  Suck ");
			ledcWrite(1, this->__config.EEF_Suck_PWM);
			break;
		case EefAction::Release:
			// Serial.print("\nGobotHouseHardware::RunM123()  Release ");
			// This will drive the extend coil to create a reversed magnetic field. 
			// ledcWrite(PIN_ENDER_COIL_2109, 128);

			break;
		case EefAction::Sleep:
			// Serial.print("\nGobotHouseHardware::RunM123()  Sleep ");
			ledcWrite(1,0);
			break;
		default:
			Serial.print("\n [Warning] GobotHouseHardware::RunM123()  Others ");
			break;
	}
}

void GobotHouseHardware::RunM84(){
	this->__EnableMotor('A',false);
	this->__EnableMotor('B',false);
}

void GobotHouseHardware::__EnableMotor(char actuator, bool enable_it){
	if (actuator == 'A')
		digitalWrite(PIN_ALPHA_ENABLE, !enable_it);
	if (actuator == 'B')
		digitalWrite(PIN_BETA_ENABLE, !enable_it);
}