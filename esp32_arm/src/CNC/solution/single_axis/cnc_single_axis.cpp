#include "cnc_single_axis.h"


void CncSingleAxis::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] CncSingleAxis::IK() is entering. ");
	FkPosition_A* fk = (FkPosition_A*)(from_fk);
	IkPosition_A* ik = (IkPosition_A*)(to_ik);

	ik->alpha = fk->A;
	Serial.print("\n[Debug] CncSingleAxis::IK() output  = ");
	Serial.print(ik->alpha);
}

void CncSingleAxis::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] CncSingleAxis::FK() is entering ");
	FkPosition_A* fk = (FkPosition_A*)(to_fk);
	IkPosition_A* ik = (IkPosition_A*)(from_ik);
	
	fk->A = ik->alpha;
	Serial.print("\n[Debug] CncSingleAxis::FK() output A = ");
	Serial.print(fk->A);
}


void CncSingleAxis::Init(CncBoardBase* board){
	Serial.print("\n[Info] CncSingleAxis::Init() is entering.");
	// this->_mechanic->Init('F');
	this->_board->EnableMotor(this->__AXIS_NAME, false);

	// this->IsInverseKinematicHoimg = false;
}

void CncSingleAxis::_SetCurrentPositionAsHome(EnumAxis homing_axis){
		//Set current position to HomePosition
		IkPosition_A ik_position;
		if (this->_config_base->IsInverseKinematicHoimg){
			// We know homed position via IK.
			Serial.print("\n[Error] CncSingleAxis::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
			// this->__current_fk_position.A = this->_mechanic->Homed_position_fk;
			this->__current_fk_position.A = this->_mechanic->HomedPosition(AXIS_ALPHA);
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_A verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis_name == this->__AXIS_NAME) {
			this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(this->__AXIS_NAME,ik_position.alpha);
		}
}

// void CncSingleAxis::RunG28(EnumAxis axis){
// 	Serial.print("[Debug] CncSingleAxis::RunG28() is entering:   " );
// 	Serial.print(axis);
// 	this->_homing_axis_name = axis;
// 	// this->_mechanic->PrintOut();
// 	// this->_board->cnc_mover->SetActuatorSpeed(this->__AXIS_NAME, this->_mechanic->Homing_speed_alpha);
// 	this->_board->cnc_mover->SetActuatorSpeed(this->__AXIS_NAME, this->_mechanic->HomingSpeed(AXIS_ALPHA));
// 	//todo :  process with IK()
// 	float long_distance_to_move = 99999;
// 	this->_board->cnc_mover->SingleActuatorMoveTo(this->__AXIS_NAME, false, long_distance_to_move);
// 	this->_board->EnableMotor(this->__AXIS_NAME, true);
	
// 	// float distance_to_move = 9999.0f * this->_mechanic->Home_is_to_max_position ;
// 	float distance_to_move = 9999.0f * this->_mechanic->HomingDir_IsToMax(AXIS_ALPHA) ;
// 	this->_board->cnc_mover->SingleActuatorMoveTo(AXIS_ALPHA, false, distance_to_move);
// }

// void CncSingleAxis::_running_G28(){
// 	if (this->_board->GetHomer(this->__AXIS_NAME)->IsTriged()){
// 		// End stop is trigered
// 		Serial.print("\n[Info] CncSingleAxis::_running_G28() Home sensor is trigger.  " );
// 		Serial.print (this->_homing_axis_name);
// 		this->_board->cnc_mover->AllActuatorsStop();

// 		//Set current position to HomePosition
// 		IkPosition_A ik_position;
// 		if (this->_config->IsInverseKinematicHoimg){
// 			// We know homed position via IK.
// 			Serial.print("\n[Error] CncSingleAxis::_running_G28() This robot does NOT impliment this function.");
// 		}
// 		else{
// 			// We know homed position via FK
// 			Serial.print("\n  [Info] Trying to get home position with EEF FK position  ");
// 			// this->__current_fk_position.A = this->_mechanic->Homed_position_fk;
// 			this->__current_fk_position.A = this->_mechanic->HomedPosition(AXIS_ALPHA);
// 			this->IK(&this->__current_fk_position, &ik_position);
// 			// verify IK by FK()
// 			FkPosition_A verifying_fk;
// 			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
// 			this->FK(&ik_position, &verifying_fk);
// 		}
// 		//Copy current ik-position to motor-position.
// 		if (this->_homing_axis_name == this->__AXIS_NAME) {
// 			this->_board->cnc_mover->SetActuatorCurrentCncPositionAs(this->__AXIS_NAME,ik_position.alpha);
// 		}
// 		this->_board->cnc_mover->SetActuatorSpeed(this->__AXIS_NAME,this->_mechanic->HomingSpeed(AXIS_ALPHA));
// 		this->State = CncState::IDLE;

// 	}else{
// 	}	
// }

void CncSingleAxis::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] CncSingleAxis::RunG1() is entering");
	Serial.print(gcode->get_command());
	this->_board->EnableMotor(this->__AXIS_NAME, true);
	if (gcode->has_letter('F')){
		float speed = gcode->get_value('F');
		this->_board->cnc_mover->SetActuatorSpeed(this->__AXIS_NAME, speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_A target_fk_a;
	IkPosition_A target_ik_a;
	target_fk_a.A = this->__current_fk_position.A;
	target_ik_a.alpha = this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit(this->__AXIS_NAME);
	bool do_ik=false;
	if (gcode->has_letter(this->__AXIS_NAME)) target_ik_a.alpha = gcode->get_value(this->__AXIS_NAME);
	// If need IK, do it now.
	if (gcode->has_letter(this->__AXIS_NAME)) {
		do_ik=true;
		target_fk_a.A = gcode->get_value(this->__AXIS_NAME);
	}
	if (do_ik) IK(&target_fk_a,&target_ik_a);

	//Prepare actuator/driver to move to next point
	this->_board->cnc_mover->SingleActuatorMoveTo(this->__AXIS_NAME,true,target_ik_a.alpha);
	//None blocking, move backgroundly.
	uint8_t abs_flag=0x01;
	this->_board->cnc_mover->AllActuatorsMoveTo(abs_flag, &target_ik_a.alpha);
	if (true){
		Serial.print("\n    [Debug] CncSingleAxis::RunG1()     (");
		this->_board->cnc_mover->GetSingleActuatorCurrentPosition_InCncUnit(this->__AXIS_NAME);
		Serial.print(")   <-- from   alpha   to -->  (");
		Serial.print(target_ik_a.alpha  );
		// Serial.print(" , ");
		// Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}
// void CncSingleAxis::_running_G1(){
//     if (this->GetDistanceToTarget_IK() < 100){   // TODO:  How to determine G1 is finished. or almost finished?
//       	this->State = CncState::IDLE;
// 		Serial.print("\n[Info] GobotHouseHardware::_running_G1() is finished. ");
//     }
// 	// Serial.println(this->GetDistanceToTarget_IK());
// 	// delay(100);
// }
// void CncSingleAxis::RunM123(uint8_t eef_channel, uint8_t eef_action){
	
// }

// void CncSingleAxis::RunM84(){
// 	this->_board->EnableMotor(this->__AXIS_NAME, false);
// }

float CncSingleAxis::GetDistanceToTarget_IK(){
	this->_board->cnc_mover->GetAbsDistanceToTarget_InCncUnit();

}


