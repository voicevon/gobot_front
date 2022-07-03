// #include "box_mover_hw.h"
#include "cnc_core_az.h"

// #define PIN_ALPHA_ENABLE 18
// #define PIN_BETA_ENABLE 16
// #define PIN_MICRIO_STEP_2 21
// #define PIN_MICRIO_STEP_1 22
// #define PIN_MICRIO_STEP_0 23

// #define STEPS_PER_RAD 123   //2048 / 2*Pi
// #define STEPS_PER_MM 345   //2048 / 2*Pi




void CncSolution_CoreAZ::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] CncSolution_CoreAZ::IK() is entering. ");
	FkPosition_ZW* fk = (FkPosition_ZW*)(from_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

	ik->alpha = (fk->Z  + fk->W);
	ik->beta = (fk->Z  - fk->W );

	Serial.print("\n[Debug] CncSolution_CoreAZ::IK() output (alpha, beta) = ");
	Serial.print(ik->alpha);
	Serial.print(" , ");
	Serial.print(ik->beta);
	Serial.print(")");
}

void CncSolution_CoreAZ::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] CncSolution_CoreAZ::FK() is entering ");
	FkPosition_ZW* fk = (FkPosition_ZW*)(to_fk);
	IkPosition_AB* ik = (IkPosition_AB*)(from_ik);
	
	fk->Z = (ik->alpha + ik->beta) / 2 ;
	fk->W = (ik->alpha - ik->beta) / 2 ;

	Serial.print("\n[Debug] CncSolution_CoreAZ::FK() output (Z, W) = ");
	Serial.print(fk->Z);
	Serial.print(" , ");
	Serial.print(fk->W);
	Serial.print(")");
}


void CncSolution_CoreAZ::Init(CncBoardBase* board){
	Serial.print("\n[Info] CncSolution_CoreAZ::Init_Linkage() is entering.");
	this->objHomeHelper_vertical = board->GetHomer(AXIS_Z);
	this->objHomeHelper_vertical = board->GetHomer(AXIS_X);
	this->_board->EnableMotor(AXIS_ALPHA, false);
	this->_board->EnableMotor(AXIS_BETA, false);

}


void CncSolution_CoreAZ::_SetCurrentPositionAsHome(EnumAxis homing_axis){
//Set current position to HomePosition
		IkPosition_AB ik_position;
		if (this->_config_base->IsInverseKinematicHoimg){
			// We know homed position via IK.
			Serial.print("\n[Error] CncSolution_CoreAZ::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
			this->__current_fk_position.Z = this->_config->Homed_position_z;
			this->__current_fk_position.W = this->_config->Homed_position_w;
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_XY verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		this->_mover->SetActuatorCurrentCncPositionAs(AXIS_ALPHA, ik_position.alpha);
		this->_mover->SetActuatorCurrentCncPositionAs(AXIS_BETA, ik_position.beta);
		
}

void CncSolution_CoreAZ::RunG28_CombinedAxis(EnumAxis axis){
	Serial.print("[Debug] CncSolution_CoreAZ::RunG28() is entering:   " );
	Serial.print(axis);
	this->_homing_axis = axis;

	this->_config->PrintOut();
	float motor_position[2];

	if (axis==AXIS_ALPHA){
		//todo :  process with IK()
		this->__homing_helper = this->objHomeHelper_angle;
		motor_position[0] = 5000000;
		motor_position[1] = 5000000;
	
	}else if (axis==AXIS_Z){
		this->__homing_helper = this->objHomeHelper_vertical;
		motor_position[0] = -5000000;
		motor_position[1] = -5000000;
	}
	this->_board->EnableMotor(AXIS_ALPHA, true);
	this->_board->EnableMotor(AXIS_BETA, true);
	this->_mover->AllActuatorsMoveTo(true, motor_position);
}

// void CncSolution_CoreAZ::_running_G28(){
// 	if (this->__homing_helper->IsTriged()){
// 		// End stop is trigered
// 		Serial.print("\n[Info] CncSolution_CoreAZ::_running_G28() Home sensor is trigger.  " );
// 		Serial.print (this->_homing_axis_name);
// 		// this->_stepControl->stop();
// 		this->_board->cnc_mover->AllActuatorsStop();

// 		//Set current position to HomePosition
// 		IkPosition_AB ik_position;
// 		if (this->_config->IsInverseKinematicHoimg){
// 			// We know homed position via IK.
// 			Serial.print("\n[Error] CncSolution_CoreAZ::_running_G28() This robot does NOT impliment this function.");
// 		}
// 		else{
// 			// We know homed position via FK
// 			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
// 			this->__current_fk_position.Z = this->_config->Homed_position_z;
// 			this->__current_fk_position.W = this->_config->Homed_position_w;
// 			this->IK(&this->__current_fk_position, &ik_position);
// 			// verify IK by FK()
// 			FkPosition_XY verifying_fk;
// 			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
// 			this->FK(&ik_position, &verifying_fk);
// 		}
// 		//Copy current ik-position to motor-position.
// 		// if (this->_homing_axis == 'Z') this->objStepper_alpha->setPosition(ik_position.alpha);
// 		// if (this->_homing_axis == 'W') this->objStepper_beta->setPosition(ik_position.beta);
// 		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_ALPHA, ik_position.alpha);
// 		this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(AXIS_BETA, ik_position.beta);
		
// 		this->State = CncState::IDLE;

// 	}else{
// 		// Endstop is not trigered

// 	}	
// }

void CncSolution_CoreAZ::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] CncSolution_CoreAZ::RunG1() is entering");
	Serial.print(gcode->get_command());
	this->_board->EnableMotor(AXIS_ALPHA, true);
	this->_board->EnableMotor(AXIS_BETA, true);
	if (gcode->has_letter('F')){
		float speed = gcode->get_value('F');
		this->_mover->SetEefSpeed(speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_ZW target_fk_zw;
	IkPosition_AB target_ik_ab;
	target_fk_zw.Z = this->__current_fk_position.Z;
	target_fk_zw.W = this->__current_fk_position.W;
	target_ik_ab.alpha = this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
	target_ik_ab.beta = this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
	bool do_ik=false;
	if (gcode->has_letter(AXIS_ALPHA)) target_ik_ab.alpha = gcode->get_value(AXIS_ALPHA);
	if (gcode->has_letter(AXIS_BETA)) target_ik_ab.beta = gcode->get_value(AXIS_BETA);

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
	float motor_position[2];
	motor_position[0] = target_ik_ab.alpha;
	motor_position[1] = target_ik_ab.beta;
	//None blocking, move backgroundly.
	this->_mover->AllActuatorsMoveTo(true, motor_position);

	if (true){
		Serial.print("\n    [Debug] CncSolution_CoreAZ::RunG1()     (");
		// Serial.print(this->objStepper_alpha->getPosition());
		Serial.print(this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
		Serial.print(",");
		// Serial.print(this->objStepper_beta->getPosition());
		Serial.print(this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
		Serial.print(")   <-- from   alpha,beta   to -->  (");
		Serial.print(target_ik_ab.alpha  );
		Serial.print(" , ");
		Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}
// void CncSolution_CoreAZ::_running_G1(){
//     if (this->GetDistanceToTarget_IK() < this->_config->max_acceleration_alpha_beta){
//       	this->State = CncState::IDLE;
// 		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
//     }
// 	// Serial.println(this->GetDistanceToTarget_IK());
// 	// delay(100);
// }
// void CncSolution_CoreAZ::RunM123(uint8_t eef_channel, EefAction eef_action){
// void CncSolution_CoreAZ::RunM123(uint8_t eef_channel, uint8_t eef_action){
	
// }



float CncSolution_CoreAZ::GetDistanceToTarget_IK(){
	return this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA) + this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
}


