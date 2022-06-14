#include "cnc_single_axis.h"


void CncSingleAxis::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] CncSingleAxis::IK() is entering. ");
	FkPosition_A* fk = (FkPosition_A*)(from_fk);
	IkPosition_A* ik = (IkPosition_A*)(to_ik);

	ik->alpha = fk->A * this->_singleAxisConfig->steps_per_rad_for_a;
	Serial.print("\n[Debug] CncSingleAxis::IK() output  = ");
	Serial.print(ik->alpha);
}

void CncSingleAxis::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] CncSingleAxis::FK() is entering ");
	FkPosition_A* fk = (FkPosition_A*)(to_fk);
	IkPosition_A* ik = (IkPosition_A*)(from_ik);
	
	fk->A = ik->alpha / this->_singleAxisConfig->steps_per_rad_for_a;
	Serial.print("\n[Debug] CncSingleAxis::FK() output A = ");
	Serial.print(fk->A);
}


CncSingleAxis::CncSingleAxis(){

}

void CncSingleAxis::Init(CncBoardBase* board, CncMachineBase* config){
	Serial.print("\n[Info] CncSingleAxis::Init() is entering.");
	this->_singleAxisConfig->Init('F');
	// pinMode(PIN_ALPHA_ENABLE, OUTPUT);
	// this->__EnableMotor('A', false);
	// this->objStepper_alpha = board->GetStepper('A');
	this->objHomeHelper_alpha = board->GetHomer('A');
	this->_board->EnableMotor('A', false);

	// CommuUart* commuUart = new CommuUart();   //TODO:  remove or rename to: OutputDevice.
	// this->commuDevice = commuUart; 
	this->objStepper_alpha->setInverseRotation(true);
	this->_home_as_inverse_kinematic = false;
}

void CncSingleAxis::HomeSingleAxis(char axis){
	Serial.print("[Debug] CncSingleAxis::HomeSingleAxis() is entering:   " );
	Serial.print(axis);
	this->_homing_axis_name = axis;
	this->_singleAxisConfig->PrintOut();
	this->objStepper_alpha->setAcceleration(this->_singleAxisConfig->Homing_acceleration_alpha);
	this->objStepper_alpha->setMaxSpeed(this->_singleAxisConfig->Homing_speed_alpha);

	if (axis=='A'){
		//todo :  process with IK()
		this->__homing_helper = this->objHomeHelper_alpha;
		this->objStepper_alpha->setTargetRel(-5000000);
	}
	// this->__EnableMotor('A', true);
	this->_board->EnableMotor('A', true);
	
	this->objStepControl->moveAsync(*this->objStepper_alpha);
}

void CncSingleAxis::_running_G28(){
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print("\n[Info] CncSingleAxis::_running_G28() Home sensor is trigger.  " );
		Serial.print (this->_homing_axis_name);
		this->objStepControl->stop();

		//Set current position to HomePosition
		IkPosition_A ik_position;
		if (this->_home_as_inverse_kinematic){
			// We know homed position via IK.
			Serial.print("\n[Error] CncSingleAxis::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
			this->__current_fk_position.A = this->_singleAxisConfig->Homed_position_fk_A;
			// this->__current_fk_position.W = this->_singleAxisConfig->Homed_position_w;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_A verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis_name == 'A') this->objStepper_alpha->setPosition(ik_position.alpha);
		// if (this->_homing_axis == 'W') this->objStepper_beta.setPosition(ik_position.beta);
		
		this->objStepper_alpha->setMaxSpeed(this->_singleAxisConfig->max_speed_alpha);
		this->objStepper_alpha->setAcceleration(this->_singleAxisConfig->max_acceleration_alpha);
		// this->objStepper_beta.setMaxSpeed(this->_singleAxisConfig->max_speed_alpha_beta);
		// this->objStepper_beta.setAcceleration(this->_singleAxisConfig->max_acceleration_alpha_beta);
		this->State = CncState::IDLE;

	}else{
	}	
}

void CncSingleAxis::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] CncSingleAxis::RunG1() is entering");
	Serial.print(gcode->get_command());
	// this->__EnableMotor('A', true);
	this->_board->EnableMotor('A', true);
	// this->__EnableMotor('B', true);
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha->setMaxSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_A target_fk_a;
	IkPosition_A target_ik_a;
	target_fk_a.A = this->__current_fk_position.A;
	target_ik_a.alpha = float(this->objStepper_alpha->getPosition()) ;
	bool do_ik=false;
	if (gcode->has_letter('A')) target_ik_a.alpha = gcode->get_value('A');
	// If need IK, do it now.
	if (gcode->has_letter('A')) {
		do_ik=true;
		target_fk_a.A = gcode->get_value('A');
	}
	if (do_ik) IK(&target_fk_a,&target_ik_a);

	//Prepare actuator/driver to move to next point
	this->objStepper_alpha->setTargetAbs(target_ik_a.alpha);
	//None blocking, move backgroundly.
	this->objStepControl->moveAsync(*this->objStepper_alpha);

	if (true){
		Serial.print("\n    [Debug] CncSingleAxis::RunG1()     (");
		Serial.print(this->objStepper_alpha->getPosition());
		Serial.print(")   <-- from   alpha   to -->  (");
		Serial.print(target_ik_a.alpha  );
		// Serial.print(" , ");
		// Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}
void CncSingleAxis::_running_G1(){
    if (this->GetDistanceToTarget_IK() < this->_singleAxisConfig->max_acceleration_alpha){
      	this->State = CncState::IDLE;
		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
    }
	// Serial.println(this->GetDistanceToTarget_IK());
	// delay(100);
}
void CncSingleAxis::RunM123(uint8_t eef_channel, EefAction eef_action){
	
}

void CncSingleAxis::RunM84(){
	// this->__EnableMotor('A',false);
	this->_board->EnableMotor('A', false);
	// this->__EnableMotor('B',false);
}

float CncSingleAxis::GetDistanceToTarget_IK(){
	return this->objStepper_alpha->getDistanceToTarget();
}

// void CncSingleAxis::__EnableMotor(char actuator, bool enable_it){
// 	if (actuator == 'A')
// 		digitalWrite(PIN_ALPHA_ENABLE, !enable_it);
// 	// if (actuator == 'B')
// 	// 	digitalWrite(PIN_BETA_ENABLE, !enable_it);
// }

