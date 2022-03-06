#include "box_mover_hw.h"

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16
// #define PIN_MICRIO_STEP_2 21
// #define PIN_MICRIO_STEP_1 22
// #define PIN_MICRIO_STEP_0 23

#define STEPS_PER_RAD 123   //2048 / 2*Pi
#define STEPS_PER_MM 345   //2048 / 2*Pi



/*
.                look from rear side.
.
.				--------------   45 degree
.				|            |  /
.				|            | /
.				|            |/
.				|      ------+------->  0 degree
.				|            |\
.				|            | \ 
.				|            |  \
.				|            |   -45 degree
.				|            |
.			  <-            <-       this is direction of positive
.            Alpha         Beta
.              ->           ->       this is direction of positive
*/

void BoxMoverHardware::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] BoxMoverHardware::IK() is entering. ");
	FkPosition_ZW* fk = (FkPosition_ZW*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	ik->alpha = (fk->Z * STEPS_PER_MM + fk->W * STEPS_PER_RAD);
	ik->beta = (fk->Z * STEPS_PER_MM - fk->W * STEPS_PER_RAD);

	Serial.print("\n[Debug] BoxMoverHardware::IK() output (alpha, beta) = ");
	Serial.print(ik->alpha);
	Serial.print(" , ");
	Serial.print(ik->beta);
	Serial.print(")");
}

void BoxMoverHardware::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] BoxMoverHardware::FK() is entering ");
	FkPosition_ZW* fk = (FkPosition_ZW*)(to_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	
	fk->Z = (ik->alpha + ik->beta) / 2 / STEPS_PER_MM;
	fk->W = (ik->alpha - ik->beta) / 2 / STEPS_PER_RAD;

	Serial.print("\n[Debug] BoxMoverHardware::FK() output (Z, W) = ");
	Serial.print(fk->Z);
	Serial.print(" , ");
	Serial.print(fk->W);
	Serial.print(")");
}


BoxMoverHardware::BoxMoverHardware(){

}

void BoxMoverHardware::Init(){
	Serial.print("\n[Info] BoxMoverHardware::Init_Linkage() is entering.");
	this->__config.Init();
	pinMode(PIN_ALPHA_ENABLE, OUTPUT);
	pinMode(PIN_BETA_ENABLE, OUTPUT);
	// pinMode(PIN_MICRIO_STEP_0, OUTPUT);
	// pinMode(PIN_MICRIO_STEP_1, OUTPUT);
	// pinMode(PIN_MICRIO_STEP_2, OUTPUT);

	this->__EnableMotor('A', false);
	this->__EnableMotor('B', false);

	// digitalWrite(PIN_MICRIO_STEP_0, LOW);
	// digitalWrite(PIN_MICRIO_STEP_1, LOW);
	// digitalWrite(PIN_MICRIO_STEP_2, LOW);
	

	CommuUart* commuUart = new CommuUart();   //TODO:  remove or rename to: OutputDevice.
	this->commuDevice = commuUart; 

	// this->objStepper_alpha.setAcceleration(MAX_ACCELERATION_ALPHPA);
	// this->objStepper_alpha.setMaxSpeed(MAX_ACCELERATION_ALPHPA);
	// this->objStepper_beta.setAcceleration(MAX_ACCELERATION_BETA);
	// this->objStepper_beta.setMaxSpeed(MAX_STEPS_PER_SECOND_BETA);
	this->objStepper_alpha.setInverseRotation(true);
	this->objStepper_beta.setInverseRotation(true);

	this->_home_as_inverse_kinematic = false;
}

void BoxMoverHardware::HomeSingleAxis(char axis){
	Serial.print("[Debug] BoxMoverHardware::HomeSingleAxis() is entering:   " );
	Serial.print(axis);
	this->_homing_axis = axis;

	this->__config.PrintOut();
	this->objStepper_alpha.setAcceleration(this->__config.Homing_acceleration_alpha_beta);
	this->objStepper_alpha.setMaxSpeed(this->__config.Homing_speed_alpha_beta);
	this->objStepper_beta.setAcceleration(this->__config.Homing_acceleration_alpha_beta);
	this->objStepper_beta.setMaxSpeed(this->__config.Homing_speed_alpha_beta);

	if (axis=='W'){
		//todo :  process with IK()
		this->__homing_helper = &this->objHomeHelper_angle;
		this->objStepper_alpha.setTargetRel(500000);
		this->objStepper_beta.setTargetRel(500000);
	}else if (axis=='Z'){
		this->__homing_helper = &this->objHomeHelper_vertical;
		this->objStepper_alpha.setTargetRel(500000);
		this->objStepper_beta.setTargetRel(-500000);	
	}
	this->__EnableMotor('A', true);
	this->__EnableMotor('B', true);
}

void BoxMoverHardware::_running_G28(){
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] BoxMoverHardware::_running_G28() Home sensor is trigger.  " );
		Serial.print (this->_homing_axis);
		this->objStepControl.stop();

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			// We know homed position via IK.
			Serial.print("\n[Error] BoxMoverHardware::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
			this->__current_fk_position.Z = this->__config.Homed_position_z;
			this->__current_fk_position.W = this->__config.Homed_position_w;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_XY verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == 'Z') this->objStepper_alpha.setPosition(ik_position.alpha);
		if (this->_homing_axis == 'W') this->objStepper_beta.setPosition(ik_position.beta);
		
		this->objStepper_alpha.setMaxSpeed(this->__config.max_speed_alpha_beta);
		this->objStepper_alpha.setAcceleration(this->__config.max_acceleration_alpha_beta);
		this->objStepper_beta.setMaxSpeed(this->__config.max_speed_alpha_beta);
		this->objStepper_beta.setAcceleration(this->__config.max_acceleration_alpha_beta);
		this->State = RobotState::IDLE;

	}else{
		// Endstop is not trigered
		// Serial.print("[Debug] Still homing\n");
		// Serial.print("<");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 
		// if (this->_homing_axis == 'W'){
		// 	//todo :  process with IK()
		// 	this->objStepper_alpha.setTargetRel(500000);
		// 	this->objStepper_beta.setTargetRel(500000);
		// 	this->__homing_helper = &this->objHomeHelper_angle;
		// }else if (this->_homing_axis == 'Z'){
		// 	// Serial.print("-");
		// 	this->objStepper_alpha.setTargetRel(500000);
		// 	this->objStepper_beta.setTargetRel(-500000);	
		// 	this->__homing_helper = &this->objHomeHelper_vertical;
		// }
	// this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);
	}	
}

void BoxMoverHardware::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] BoxMoverHardware::RunG1() is entering");
	Serial.print(gcode->get_command());
	this->__EnableMotor('A', true);
	this->__EnableMotor('B', true);
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
		this->objStepper_beta.setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_ZW target_fk_zw;
	IkPosition_AB target_ik_ab;
	target_fk_zw.Z = this->__current_fk_position.Z;
	target_fk_zw.W = this->__current_fk_position.W;
	target_ik_ab.alpha = float(this->objStepper_alpha.getPosition()) ;
	target_ik_ab.beta = float(this->objStepper_beta.getPosition());
	bool do_ik=false;
	if (gcode->has_letter('A')) target_ik_ab.alpha = gcode->get_value('A');
	if (gcode->has_letter('B')) target_ik_ab.beta = gcode->get_value('B');

	// If need IK, do it now.
	if (gcode->has_letter('Z')) {
		do_ik=true;
		target_fk_zw.Z = gcode->get_value('Z');
	}
	if (gcode->has_letter('W')){
		do_ik=true;
		target_fk_zw.W = gcode->get_value('W');
	}
	if (do_ik) IK(&target_fk_zw,&target_ik_ab);

	//Prepare actuator/driver to move to next point
	this->objStepper_alpha.setTargetAbs(target_ik_ab.alpha);
	this->objStepper_beta.setTargetAbs(target_ik_ab.beta);
	//None blocking, move backgroundly.
	this->objStepControl.moveAsync(this->objStepper_alpha, this->objStepper_beta);

	if (true){
		Serial.print("\n    [Debug] BoxMoverHardware::RunG1()     (");
		Serial.print(this->objStepper_alpha.getPosition());
		Serial.print(",");
		Serial.print(this->objStepper_beta.getPosition());
		Serial.print(")   <-- from   alpha,beta   to -->  (");
		Serial.print(target_ik_ab.alpha  );
		Serial.print(" , ");
		Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}
void BoxMoverHardware::_running_G1(){
    if (this->GetDistanceToTarget_IK() < this->__config.max_acceleration_alpha_beta){
      	this->State = RobotState::IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
void BoxMoverHardware::RunM123(uint8_t eef_channel, EefAction eef_action){
	
}

void BoxMoverHardware::RunM84(){
	this->__EnableMotor('A',false);
	this->__EnableMotor('B',false);
}

float BoxMoverHardware::GetDistanceToTarget_IK(){
	return this->objStepper_alpha.getDistanceToTarget() + this->objStepper_beta.getDistanceToTarget();
}

void BoxMoverHardware::__EnableMotor(char actuator, bool enable_it){
	if (actuator == 'A')
		digitalWrite(PIN_ALPHA_ENABLE, !enable_it);
	if (actuator == 'B')
		digitalWrite(PIN_BETA_ENABLE, !enable_it);
}

