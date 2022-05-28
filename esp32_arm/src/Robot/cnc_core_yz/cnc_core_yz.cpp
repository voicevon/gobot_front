// #include "box_carrier_hw.h"
#include "cnc_core_yz.h"


void BoxCarrierHardware::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] BoxCarrierHardware::IK() is entering. ");
	FkPosition_YZ* fk = (FkPosition_YZ*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	ik->alpha = (fk->Z * this->__config.steps_per_mm_for_z + fk->Y * this->__config.steps_per_mm_for_y);
	ik->beta = (fk->Z * this->__config.steps_per_mm_for_z - fk->Y * this->__config.steps_per_mm_for_y);

	Serial.print("\n[Debug] BoxCarrierHardware::IK() output (alpha, beta) = ");
	Serial.print(ik->alpha);
	Serial.print(" , ");
	Serial.print(ik->beta);
	Serial.print(")");
}

void BoxCarrierHardware::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] BoxCarrierHardware::FK() is entering ");
	FkPosition_YZ* fk = (FkPosition_YZ*)(to_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	
	fk->Z = (ik->alpha + ik->beta) / 2 / this->__config.steps_per_mm_for_z;
	fk->Y = (ik->alpha - ik->beta) / 2 / this->__config.steps_per_mm_for_y;

	Serial.print("\n[Debug] BoxCarrierHardware::FK() output (Z, Y) = ");
	Serial.print(fk->Z);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}



// BoxCarrierHardware::BoxCarrierHardware(uint8_t pin_alpha_enable, uint8_t pin_beta_enable){
// 	pinMode(pin_alpha_enable, OUTPUT);
// 	pinMode(pin_beta_enable, OUTPUT);
// 	this->__pin_alpha_enable = pin_alpha_enable;
// 	this->__pin_beta_enable = pin_beta_enable;

// 	this->__mcp23018 = nullptr;
// 	this->__EnableMotor('A', false);
// 	this->__EnableMotor('B', false);
// }

// BoxCarrierHardware::BoxCarrierHardware(Adafruit_MCP23X17* mcp_23018, uint8_t pin_alpha_enable, uint8_t pin_beta_enable){
// 	this->__mcp23018 = mcp_23018;
// 	this->__mcp23018->pinMode(pin_alpha_enable, OUTPUT);
// 	this->__mcp23018->pinMode(pin_beta_enable, OUTPUT);
// 	this->__EnableMotor('A', false);
// 	this->__EnableMotor('B', false);
// }

void BoxCarrierHardware::LinkStepper(Stepper* alpha, Stepper* beta){
	this->stepper_alpha = alpha;
	this->stepper_beta = beta;
}

void BoxCarrierHardware::LinkHomer(SingleAxisHomer* homer_z, SingleAxisHomer* homer_y){
	this->objHomeHelper_vertical = homer_z;
	this->objHomeHelper_y = homer_y;
}

void BoxCarrierHardware::InitRobot(){
	Serial.print("\n[Info] BoxCarrierHardware::Init_Linkage() is entering.");
	this->__config.Init();


	// CommuUart* commuUart = new CommuUart();   //TODO:  remove or rename to: OutputDevice.
	// this->commuDevice = commuUart; 

	this->stepper_alpha->setInverseRotation(true);
	this->stepper_beta->setInverseRotation(true);

	this->_home_as_inverse_kinematic = false;
}

void BoxCarrierHardware::HomeSingleAxis(char axis){
	Serial.print("[Debug] BoxCarrierHardware::HomeSingleAxis() is entering:   " );
	Serial.print(axis);
	this->_homing_axis = axis;

	this->__config.PrintOut();
	this->stepper_alpha->setAcceleration(this->__config.Homing_acceleration_alpha_beta);
	this->stepper_alpha->setMaxSpeed(this->__config.Homing_speed_alpha_beta);
	this->stepper_beta->setAcceleration(this->__config.Homing_acceleration_alpha_beta);
	this->stepper_beta->setMaxSpeed(this->__config.Homing_speed_alpha_beta);

	if (axis=='Y'){
		//todo :  process with IK()
		this->__homing_helper = this->objHomeHelper_y;
		this->stepper_alpha->setTargetRel(5000000);
		this->stepper_beta->setTargetRel(5000000);
	}else if (axis=='Z'){
		this->__homing_helper = this->objHomeHelper_vertical;
		this->stepper_alpha->setTargetRel(-5000000);
		this->stepper_beta->setTargetRel(5000000);	
	}

	this->__EnableMotor('A', true);
	this->__EnableMotor('B', true);
	this->objStepControl->moveAsync(*this->stepper_alpha, *this->stepper_beta);
}

void BoxCarrierHardware::_running_G28(){
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] BoxCarrierHardware::_running_G28() Home sensor is trigger.  " );
		Serial.print (this->_homing_axis);
		this->objStepControl->stop();

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_home_as_inverse_kinematic){
			// We know homed position via IK.
			Serial.print("\n[Error] BoxCarrierHardware::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
			this->__current_fk_position.Z = this->__config.Homed_position_z;
			this->__current_fk_position.Y = this->__config.Homed_position_y;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_YZ verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == 'Z') this->stepper_alpha->setPosition(ik_position.alpha);
		if (this->_homing_axis == 'Y') this->stepper_beta->setPosition(ik_position.beta);
		
		this->stepper_alpha->setMaxSpeed(this->__config.max_speed_alpha_beta);
		this->stepper_alpha->setAcceleration(this->__config.max_acceleration_alpha_beta);
		this->stepper_beta->setMaxSpeed(this->__config.max_speed_alpha_beta);
		this->stepper_beta->setAcceleration(this->__config.max_acceleration_alpha_beta);
		this->State = RobotState::IDLE;

	}else{
		// Endstop is not trigered
		// Serial.print("[Debug] Still homing\n");
		// Serial.print("<");
		// We are going to move a long long distance with async mode(None blocking).
		// When endstop is trigered, must stop the moving. 
		// if (this->_homing_axis == 'W'){
		// 	//todo :  process with IK()
		// 	this->stepper_alpha.setTargetRel(500000);
		// 	this->stepper_beta.setTargetRel(500000);
		// 	this->__homing_helper = &this->objHomeHelper_angle;
		// }else if (this->_homing_axis == 'Z'){
		// 	// Serial.print("-");
		// 	this->stepper_alpha.setTargetRel(500000);
		// 	this->stepper_beta.setTargetRel(-500000);	
		// 	this->__homing_helper = &this->objHomeHelper_vertical;
		// }
	// this->objStepControl.moveAsync(this->stepper_alpha, this->stepper_beta);
	}	
}

void BoxCarrierHardware::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] BoxCarrierHardware::RunG1() is entering");
	Serial.print(gcode->get_command());
	this->__EnableMotor('A', true);
	this->__EnableMotor('B', true);
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->stepper_alpha->setMaxSpeed(speed);
		this->stepper_beta->setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_YZ target_fk_yz;
	IkPosition_AB target_ik_ab;
	target_fk_yz.Z = this->__current_fk_position.Z;
	target_fk_yz.Y = this->__current_fk_position.Y;
	target_ik_ab.alpha = float(this->stepper_alpha->getPosition()) ;
	target_ik_ab.beta = float(this->stepper_beta->getPosition());
	bool do_ik=false;
	if (gcode->has_letter('A')) target_ik_ab.alpha = gcode->get_value('A');
	if (gcode->has_letter('B')) target_ik_ab.beta = gcode->get_value('B');

	// If need IK, do it now.
	if (gcode->has_letter('Z')) {
		do_ik=true;
		target_fk_yz.Z = gcode->get_value('Z');
	}
	if (gcode->has_letter('Y')){
		do_ik=true;
		target_fk_yz.Y = gcode->get_value('Y');
	}
	if (do_ik) IK(&target_fk_yz,&target_ik_ab);

	//Prepare actuator/driver to move to next point
	this->stepper_alpha->setTargetAbs(target_ik_ab.alpha);
	this->stepper_beta->setTargetAbs(target_ik_ab.beta);
	//None blocking, move backgroundly.
	this->objStepControl->moveAsync(*this->stepper_alpha, *this->stepper_beta);

	if (true){
		Serial.print("\n    [Debug] BoxCarrierHardware::RunG1()     (");
		Serial.print(this->stepper_alpha->getPosition());
		Serial.print(",");
		Serial.print(this->stepper_beta->getPosition());
		Serial.print(")   <-- from   alpha,beta   to -->  (");
		Serial.print(target_ik_ab.alpha  );
		Serial.print(" , ");
		Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}

void BoxCarrierHardware::_running_G1(){
    if (this->GetDistanceToTarget_IK() < this->__config.max_acceleration_alpha_beta){
      	this->State = RobotState::IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}

void BoxCarrierHardware::RunM123(uint8_t eef_channel, EefAction eef_action){
	
}

void BoxCarrierHardware::RunM84(){
	this->__EnableMotor('A',false);
	this->__EnableMotor('B',false);
}

float BoxCarrierHardware::GetDistanceToTarget_IK(){
	return this->stepper_alpha->getDistanceToTarget() + this->stepper_beta->getDistanceToTarget();
}

void BoxCarrierHardware::__EnableMotor(char actuator, bool enable_it){
		if (actuator == 'A'){
			this->__board->EnableMotor_alpha(enable_it);
		}
		else if (actuator == 'B'){
			this->__board->EnableMotor_beta(enable_it);
		}
		else{
			Serial.print("[Warn] BoxCarrierHardware::__EnableMotor()   unkown actuator = ");
			Serial.println(actuator);
		}
}
