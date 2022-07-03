#include "cnc_core_yz.h"


void Cnc_CoreYZ::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] Cnc_CoreYZ::IK() is entering. ");
	FkPosition_YZ* fk = (FkPosition_YZ*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	ik->alpha = (fk->Z  + fk->Y );
	ik->beta = (fk->Z - fk->Y );

	Serial.print("\n[Debug] Cnc_CoreYZ::IK() output (alpha, beta) = ");
	Serial.print(ik->alpha);
	Serial.print(" , ");
	Serial.print(ik->beta);
	Serial.print(")");
}

void Cnc_CoreYZ::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] Cnc_CoreYZ::FK() is entering ");
	FkPosition_YZ* fk = (FkPosition_YZ*)(to_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	
	fk->Z = (ik->alpha + ik->beta) / 2;
	fk->Y = (ik->alpha - ik->beta) / 2;

	Serial.print("\n[Debug] Cnc_CoreYZ::FK() output (Z, Y) = ");
	Serial.print(fk->Z);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}


void Cnc_CoreYZ::Init(CncBoardBase* board){
	Serial.print("\n[Info] Cnc_CoreYZ::Init_Linkage() is entering.");
	this->_cncMachine = (CncSolution_CoreYZConfigBase*)(this->_board->GetCncConfig());
	this->_board = board;

	this->objHomeHelper_y = board->GetHomer(AXIS_Y);
	this->objHomeHelper_vertical = board->GetHomer(AXIS_Z);


}

void Cnc_CoreYZ::RunG28_CombinedAxis(EnumAxis axis){
	Serial.print("[Debug] Cnc_CoreYZ::RunG28() is entering:   " );
	Serial.println(axis);
	this->_homing_axis_name = axis;
	this->_cncMachine->PrintOut("Cnc_CoreYZ::RunG28()");
	// Serial.println(this->stepper_alpha->getPosition());
	// this->stepper_alpha->setAcceleration(this->_cncMachine->Homing_acceleration_alpha_beta);
	// this->stepper_alpha->setMaxSpeed(this->_cncMachine->Homing_speed_alpha_beta);
	// this->stepper_beta->setAcceleration(this->_cncMachine->Homing_acceleration_alpha_beta);
	// this->stepper_beta->setMaxSpeed(this->_cncMachine->Homing_speed_alpha_beta);
	float motor_position[2];

	if (axis=='Y'){
		//todo :  process with IK()
		this->__homing_helper = this->objHomeHelper_y;
		// this->stepper_alpha->setTargetRel(-5000000);
		// this->stepper_beta->setTargetRel(5000000);
		motor_position[0] = -5000000;
		motor_position[1] = 5000000;
	}else if (axis=='Z'){
		this->__homing_helper = this->objHomeHelper_vertical;
		// this->stepper_alpha->setTargetRel(-5000000);
		// this->stepper_beta->setTargetRel(-5000000);
		motor_position[0]=-5000000;
		motor_position[1]=-5000000;	
	}
	this->_mover->AllActuatorsMoveTo(false, motor_position);

	// this->_board->EnableMotor('A', true);
	// this->_board->EnableMotor('B',true);

	// this->_stepControl->moveAsync(*this->stepper_alpha, *this->stepper_beta);
}

void Cnc_CoreYZ::_SetCurrentPositionAsHome(EnumAxis homing_axis){

		//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_config_base->IsInverseKinematicHoimg){
			// We know homed position via IK.
			Serial.print("\n[Error] Cnc_CoreYZ::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
			this->__current_fk_position.Z = this->_cncMachine->Homed_position_z;
			this->__current_fk_position.Y = this->_cncMachine->Homed_position_y;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_YZ verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis_name == 'Z') {
			this->_mover->SingleActuatorMoveTo(AXIS_ALPHA, true, ik_position.alpha);
		}
		if (this->_homing_axis_name == 'Y') {
			this->_mover->SingleActuatorMoveTo(AXIS_BETA, true, ik_position.beta);
		}
}

// void Cnc_CoreYZ::_running_G28(){
// 	if (this->__homing_helper->IsTriged()){
// 		// End stop is trigered
// 		Serial.print("\n[Info] Cnc_CoreYZ::_running_G28() Home sensor is trigger.  " );
// 		Serial.print (this->_homing_axis_name);
// 		// this->_stepControl->stop();
// 		this->_board->cnc_mover->AllActuatorsStop();

// 		//Set current position to HomePosition
// 		IkPosition_AB ik_position;
// 		if (this->_config->IsInverseKinematicHoimg){
// 			// We know homed position via IK.
// 			Serial.print("\n[Error] Cnc_CoreYZ::_running_G28() This robot does NOT impliment this function.");
// 		}
// 		else{
// 			// We know homed position via FK
// 			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
// 			this->__current_fk_position.Z = this->_cncMachine->Homed_position_z;
// 			this->__current_fk_position.Y = this->_cncMachine->Homed_position_y;
// 			this->IK(&this->__current_fk_position, &ik_position);
// 			// verify IK by FK()
// 			FkPosition_YZ verifying_fk;
// 			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
// 			this->FK(&ik_position, &verifying_fk);
// 		}
// 		//Copy current ik-position to motor-position.
// 		if (this->_homing_axis_name == 'Z') {
// 			this->_board->cnc_mover->SingleActuatorMoveTo(AXIS_ALPHA, true, ik_position.alpha);
// 		}
// 		if (this->_homing_axis_name == 'Y') {
// 			this->_board->cnc_mover->SingleActuatorMoveTo(AXIS_BETA, true, ik_position.beta);
// 		}
		
// 		this->State = CncState::IDLE;

// 	}else{
// 		// Endstop is not trigered
// 		// Serial.print("[Debug] Still homing\n");
// 		// Serial.print("<");
// 		// We are going to move a long long distance with async mode(None blocking).
// 		// When endstop is trigered, must stop the moving. 
// 		// if (this->_homing_axis == 'W'){
// 		// 	//todo :  process with IK()
// 		// 	this->stepper_alpha.setTargetRel(500000);
// 		// 	this->stepper_beta.setTargetRel(500000);
// 		// 	this->__homing_helper = &this->objHomeHelper_angle;
// 		// }else if (this->_homing_axis == 'Z'){
// 		// 	// Serial.print("-");
// 		// 	this->stepper_alpha.setTargetRel(500000);
// 		// 	this->stepper_beta.setTargetRel(-500000);	
// 		// 	this->__homing_helper = &this->objHomeHelper_vertical;
// 		// }
// 	// this->objStepControl.moveAsync(this->stepper_alpha, this->stepper_beta);
// 	}	
// }

void Cnc_CoreYZ::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] Cnc_CoreYZ::RunG1() is entering");
	Serial.print(gcode->get_command());
	this->_board->EnableMotor(AXIS_ALPHA, true);
	this->_board->EnableMotor(AXIS_BETA, true);
	if (gcode->has_letter('F')){
		float speed = gcode->get_value('F');
		// this->stepper_alpha->setMaxSpeed(speed);
		// this->stepper_beta->setMaxSpeed(speed);
		this->_mover->SetEefSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_YZ target_fk_yz;
	IkPosition_AB target_ik_ab;
	target_fk_yz.Z = this->__current_fk_position.Z;
	target_fk_yz.Y = this->__current_fk_position.Y;
	// target_ik_ab.alpha = float(this->stepper_alpha->getPosition()) ;
	// target_ik_ab.beta = float(this->stepper_beta->getPosition());
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
	// this->stepper_alpha->setTargetAbs(target_ik_ab.alpha);
	// this->stepper_beta->setTargetAbs(target_ik_ab.beta);
	// //None blocking, move backgroundly.
	// this->_stepControl->moveAsync(*this->stepper_alpha, *this->stepper_beta);
	float target_motor_position[2];
	target_motor_position[0] = target_ik_ab.alpha;
	target_motor_position[1] = target_ik_ab.beta;
	this->_mover->AllActuatorsMoveTo(true, target_motor_position);

	if (true){
		Serial.print("\n    [Debug] Cnc_CoreYZ::RunG1()     (");
		// Serial.print(this->stepper_alpha->getPosition());
		Serial.print(",");
		// Serial.print(this->stepper_beta->getPosition());
		Serial.print(")   <-- from   alpha,beta   to -->  (");
		Serial.print(target_ik_ab.alpha  );
		Serial.print(" , ");
		Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}



float Cnc_CoreYZ::GetDistanceToTarget_IK(){
	// return this->stepper_alpha->getDistanceToTarget() + this->stepper_beta->getDistanceToTarget();
	return this->_mover->GetAbsDistanceToTarget_InCncUnit();
}



