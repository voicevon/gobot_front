#include "spring_maker_hw.h"

#define PIN_ALPHA_ENABLE 18



void SpringMakerHardware::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] SpringMakerHardware::IK() is entering. ");
	FkPosition_A* fk = (FkPosition_A*)(from_fk);
	IkPosition_A* ik = (IkPosition_A*)(to_ik);

	ik->alpha = fk->A * this->__config.steps_per_rad_for_a;
	Serial.print("\n[Debug] SpringMakerHardware::IK() output  = ");
	Serial.print(ik->alpha);
}

void SpringMakerHardware::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] SpringMakerHardware::FK() is entering ");
	FkPosition_A* fk = (FkPosition_A*)(to_fk);
	IkPosition_A* ik = (IkPosition_A*)(from_ik);
	
	fk->A = ik->alpha / this->__config.steps_per_rad_for_a;
	Serial.print("\n[Debug] SpringMakerHardware::FK() output A = ");
	Serial.print(fk->A);
}


SpringMakerHardware::SpringMakerHardware(){
	this->InitRobot();
}

void SpringMakerHardware::InitRobot(){
	Serial.print("\n[Info] SpringMakerHardware::InitRobot() is entering.");
	this->__config.Init();
	pinMode(PIN_ALPHA_ENABLE, OUTPUT);
	this->__EnableMotor('A', false);

	CommuUart* commuUart = new CommuUart();   //TODO:  remove or rename to: OutputDevice.
	this->commuDevice = commuUart; 
	this->objStepper_alpha.setInverseRotation(true);
	this->_home_as_inverse_kinematic = false;
}

void SpringMakerHardware::HomeSingleAxis(char axis){
	Serial.print("[Debug] SpringMakerHardware::HomeSingleAxis() is entering:   " );
	Serial.print(axis);
	this->_homing_axis = axis;
	this->__config.PrintOut();
	this->objStepper_alpha.setAcceleration(this->__config.Homing_acceleration_alpha);
	this->objStepper_alpha.setMaxSpeed(this->__config.Homing_speed_alpha);

	if (axis=='A'){
		//todo :  process with IK()
		this->__homing_helper = &this->objHomeHelper_alpha;
		this->objStepper_alpha.setTargetRel(-5000000);
	}
	this->__EnableMotor('A', true);
	this->objStepControl.moveAsync(this->objStepper_alpha);
}

void SpringMakerHardware::_running_G28(){
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] SpringMakerHardware::_running_G28() Home sensor is trigger.  " );
		Serial.print (this->_homing_axis);
		this->objStepControl.stop();

		//Set current position to HomePosition
		IkPosition_A ik_position;
		if (this->_home_as_inverse_kinematic){
			// We know homed position via IK.
			Serial.print("\n[Error] SpringMakerHardware::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
			this->__current_fk_position.A = this->__config.Homed_position_fk_A;
			// this->__current_fk_position.W = this->__config.Homed_position_w;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_A verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == 'A') this->objStepper_alpha.setPosition(ik_position.alpha);
		// if (this->_homing_axis == 'W') this->objStepper_beta.setPosition(ik_position.beta);
		
		this->objStepper_alpha.setMaxSpeed(this->__config.max_speed_alpha);
		this->objStepper_alpha.setAcceleration(this->__config.max_acceleration_alpha);
		// this->objStepper_beta.setMaxSpeed(this->__config.max_speed_alpha_beta);
		// this->objStepper_beta.setAcceleration(this->__config.max_acceleration_alpha_beta);
		this->State = RobotState::IDLE;

	}else{
	}	
}

void SpringMakerHardware::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] SpringMakerHardware::RunG1() is entering");
	Serial.print(gcode->get_command());
	this->__EnableMotor('A', true);
	// this->__EnableMotor('B', true);
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_A target_fk_a;
	IkPosition_A target_ik_a;
	target_fk_a.A = this->__current_fk_position.A;
	target_ik_a.alpha = float(this->objStepper_alpha.getPosition()) ;
	bool do_ik=false;
	if (gcode->has_letter('A')) target_ik_a.alpha = gcode->get_value('A');
	// If need IK, do it now.
	if (gcode->has_letter('A')) {
		do_ik=true;
		target_fk_a.A = gcode->get_value('A');
	}
	if (do_ik) IK(&target_fk_a,&target_ik_a);

	//Prepare actuator/driver to move to next point
	this->objStepper_alpha.setTargetAbs(target_ik_a.alpha);
	//None blocking, move backgroundly.
	this->objStepControl.moveAsync(this->objStepper_alpha);

	if (true){
		Serial.print("\n    [Debug] SpringMakerHardware::RunG1()     (");
		Serial.print(this->objStepper_alpha.getPosition());
		Serial.print(")   <-- from   alpha   to -->  (");
		Serial.print(target_ik_a.alpha  );
		// Serial.print(" , ");
		// Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}
void SpringMakerHardware::_running_G1(){
    if (this->GetDistanceToTarget_IK() < this->__config.max_acceleration_alpha){
      	this->State = RobotState::IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
void SpringMakerHardware::RunM123(uint8_t eef_channel, EefAction eef_action){
	
}

void SpringMakerHardware::RunM84(){
	this->__EnableMotor('A',false);
	// this->__EnableMotor('B',false);
}

float SpringMakerHardware::GetDistanceToTarget_IK(){
	return this->objStepper_alpha.getDistanceToTarget();
}

void SpringMakerHardware::__EnableMotor(char actuator, bool enable_it){
	if (actuator == 'A')
		digitalWrite(PIN_ALPHA_ENABLE, !enable_it);
	// if (actuator == 'B')
	// 	digitalWrite(PIN_BETA_ENABLE, !enable_it);
}

