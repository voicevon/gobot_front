#include "gobot_house_hw.h"

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16
#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23


#define PIN_ENDER_COIL_2109 32
#define PIN_ENDER_COIL_EXT_2109 33
// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/

// }
void GobotHouseHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);
	float rr1= fk->X * fk->X + fk->Y * fk->Y;

	float beta = PI - acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X / r1);    // [0..PI]
	if (fk->Y < 0)  alpha_eef = 2 * PI - alpha_eef;  // [0..2*PI]
	float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B)/( 2*LINK_A * r1));  //[0..PI]
	float alpha = alpha_eef - alpha_link;  //[-PI.. + 2*PI]?
	if (alpha > PI * 10 /180) alpha -= 2 * PI;   // [-330..+10 ] in degree

	Serial.print("\n[Debug] GobotHouseHardware::IK() from (X,Y)=(");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")\n    Inverse kinematic angle degree of origin (alpha_eef, alpha_link, ik->alpha)=( ");
	Serial.print(alpha_eef * 180 / PI);
	Serial.print(" , ");
	Serial.print(alpha_link * 180 / PI);
	Serial.print(" , ");
	Serial.print(alpha * 180 / PI);
	Serial.print(")");

	// if (alpha < 0) alpha +=

	ik->alpha = alpha * STEPS_PER_RAD_ALPHA;
	ik->beta =  beta * STEPS_PER_RAD_BETA;

	Serial.print("\n    Inverse Kinematic result in angle degree (alpha, beta)= ");
	Serial.print(alpha * 180 / PI);
	Serial.print(" , ");
	Serial.print(beta * 180 / PI);
	Serial.print(")");
}

void GobotHouseHardware::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);
	float rad_beta = ik->beta / STEPS_PER_RAD_BETA;
	float rad_eef = rad_beta + ik->alpha /STEPS_PER_RAD_ALPHA;
	float rad_alpha = ik->alpha /STEPS_PER_RAD_ALPHA;
	fk->X = LINK_A * cosf(rad_alpha) + LINK_B * cosf(rad_eef);
	fk->Y = LINK_A * sinf(rad_alpha) + LINK_B * sinf(rad_eef);

	Serial.print("\n\n[Debug] GobotHouseHardware::FK()  in degree from (alpha,beta) =(");
	Serial.print(rad_alpha * 180 / PI);
	Serial.print(" , ");
	Serial.print(rad_beta * 180 / PI);
	Serial.print(") \n     Forward Kinematic result:  (X,Y)= (");
	Serial.print(fk->X);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}

GobotHouseHardware::GobotHouseHardware(){
	this->__config.Init();
}



bool GobotHouseHardware::GetCurrentPosition(FkPositionBase* position_fk){
	position_fk = & this->__current_fk_position;
	return true;
}


void GobotHouseHardware::init_gpio(){
	pinMode(PIN_ALPHA_ENABLE, OUTPUT);
	pinMode(PIN_BETA_ENABLE, OUTPUT);
	pinMode(PIN_MICRIO_STEP_0, OUTPUT);
	pinMode(PIN_MICRIO_STEP_1, OUTPUT);
	pinMode(PIN_MICRIO_STEP_2, OUTPUT);
	pinMode(PIN_ENDER_COIL_2109, OUTPUT);
	pinMode(PIN_ENDER_COIL_EXT_2109, OUTPUT);

	digitalWrite(PIN_ALPHA_ENABLE, LOW);
	digitalWrite(PIN_BETA_ENABLE, LOW);
	digitalWrite(PIN_MICRIO_STEP_0, LOW);
	digitalWrite(PIN_MICRIO_STEP_1, LOW);
	digitalWrite(PIN_MICRIO_STEP_2, LOW);
	digitalWrite(PIN_ENDER_COIL_2109, LOW);
	digitalWrite(PIN_ENDER_COIL_EXT_2109, LOW);

}
void GobotHouseHardware::Init_Linkage(){
	init_gpio();
	this->commuDevice = &this->objCommuUart; 
	this->objStepper_alpha.setAcceleration(MAX_ACCELERATION_ALPHPA);
	this->objStepper_alpha.setMaxSpeed(MAX_ACCELERATION_ALPHPA);
	this->objStepper_beta.setAcceleration(MAX_ACCELERATION_BETA);
	this->objStepper_beta.setMaxSpeed(MAX_STEPS_PER_SECOND_BETA);
	this->objStepper_alpha.setInverseRotation(true);
	this->objStepper_beta.setInverseRotation(false);

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
	Serial.print("\n[Debug] GobotHouseHardware::RunG1()   ");
	Serial.print(gcode->get_command());
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
	if (gcode->has_letter('A')) target_ik_ab.alpha = gcode->get_value('A') * STEPS_PER_RAD_ALPHA * PI/ 180;
	if (gcode->has_letter('B')) target_ik_ab.beta = gcode->get_value('B') * STEPS_PER_RAD_BETA * PI / 180;

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
	if(gcode->has_letter('R')) target_ik_ab.alpha = STEPS_PER_ROUND_MOTOR * gcode->get_value('R');
	//Prepare actuator/driver to move to next point
	this->objStepper_alpha.setTargetAbs(target_ik_ab.alpha );
	this->objStepper_beta.setTargetAbs(target_ik_ab.beta);
	//None blocking, move backgroundly.
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);

	if (true){
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
    if (this->GetDistanceToTarget_IK() < (MAX_ACCELERATION_ALPHPA + MAX_ACCELERATION_BETA)/64){
      	this->State = IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
void GobotHouseHardware::HomeSingleAxis(char axis){
	Serial.print("[Debug] GobotHouseHardware::HomeSingleAxis() is entering\n" );
	Serial.print(axis);
	this->_homing_axis = axis;
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

}

void GobotHouseHardware::_running_G28(){
	// Serial.print("[Debug] GobotHouseHardware::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] GobotHouseHardware::_running_G28() Home sensor is trigger.  " );
		Serial.print (this->_homing_axis);
		this->objStepControl.stop();

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			Serial.print("\n   [Info] Trying to get home position from actuator position  ");
			ik_position.alpha = PI * this->__config.Homed_position_alpha_in_degree * STEPS_PER_RAD_ALPHA /180;
			ik_position.beta = PI * this->__config.Homed_position_beta_in_degree * STEPS_PER_RAD_BETA /180;
			this->FK(&ik_position, &this->__current_fk_position);
			// verify FK by IK()
			IkPosition_AB verifying_ik;
			Serial.print("\n\n[Info] Please verify the below output ======================  ");
			this->IK(&this->__current_fk_position, &verifying_ik);
		}
		else{
			Serial.print("\n [Info] Trying to get home position with EEF position  ");
			this->__current_fk_position.X = this->__config.Homed_position_x;
			this->__current_fk_position.Y = this->__config.Homed_position_y;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_XY verifying_fk;
			Serial.print("\n   [Info] Please verify the below output ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == 'A') this->objStepper_alpha.setPosition(ik_position.alpha);
		if (this->_homing_axis == 'B') this->objStepper_beta.setPosition(ik_position.beta);
		
		this->objStepper_alpha.setMaxSpeed(MAX_STEPS_PER_SECOND_ALPHA);
		this->objStepper_alpha.setAcceleration(MAX_ACCELERATION_ALPHPA);
		this->objStepper_beta.setMaxSpeed(MAX_STEPS_PER_SECOND_BETA);
		this->objStepper_beta.setAcceleration(MAX_ACCELERATION_BETA);
		this->State = IDLE;

	}else{
		// Endstop is not trigered
		// Serial.print("[Debug] Still homing\n");
		// Serial.print("<");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 
		this->__homing_stepper->setTargetRel(50000);
		this->objStepControl.moveAsync(*this->__homing_stepper);

	}
}


