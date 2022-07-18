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


void CncSingleAxis::_SetCurrentPositionAsHome(EnumAxis homing_axis){
		//Set current position to HomePosition
		IkPosition_A ik_position;
		if (this->_config_base->IsInverseKinematicHoimg){
			// We know homed position via IK.
			Serial.print("\n[Error] CncSingleAxis::_running_G28() This robot does NOT impliment this function.");
		}
		else{
			// We know homed position via FK
			Logger::Info("CncSingleAxis::_SetCurrentPositionAsHome()  Trying to get home position with EEF FK position  ");
			Logger::Print("Config.HomedPosition()", this->_config_base->HomedPosition(AXIS_ALPHA));
			this->__current_fk_position.A = this->_config_base->HomedPosition(AXIS_ALPHA);
			this->IK(&this->__current_fk_position, &ik_position);
			// verify IK by FK()
			FkPosition_A verifying_fk;
			Serial.print("\n   [Info] Please verify: FK->IK->FK  ");
			this->FK(&ik_position, &verifying_fk);
		}
		//Copy current ik-position to motor-position.
		if (this->_homing_axis == this->__AXIS_NAME) {
			this->_mover->SetActuatorCurrentCncPositionAs(this->__AXIS_NAME,ik_position.alpha);
		}else{
			Logger::Halt("CncSingleAxis::_SetCurrentPositionAsHome()");
		}
}


void CncSingleAxis::RunG1(Gcode* gcode) {
	Serial.print("\n[Debug] CncSingleAxis::RunG1() is entering");
	Serial.print(gcode->get_command());
	this->_board->EnableMotor(this->__AXIS_NAME, true);
	if (gcode->has_letter('F')){
		float speed = gcode->get_value('F');
		this->_mover->SetActuatorSpeed(this->__AXIS_NAME, speed);
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_A target_fk_a;
	IkPosition_A target_ik_a;
	target_fk_a.A = this->__current_fk_position.A;
	target_ik_a.alpha = this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(this->__AXIS_NAME);
	bool do_ik=false;
	if (gcode->has_letter(this->__AXIS_NAME)) target_ik_a.alpha = gcode->get_value(this->__AXIS_NAME);
	// If need IK, do it now.
	if (gcode->has_letter(this->__AXIS_NAME)) {
		do_ik=true;
		target_fk_a.A = gcode->get_value(this->__AXIS_NAME);
	}
	if (do_ik) IK(&target_fk_a,&target_ik_a);

	//Prepare actuator/driver to move to next point
	this->_mover->SingleActuatorMoveTo(this->__AXIS_NAME,true,target_ik_a.alpha);
	//None blocking, move backgroundly.
	uint8_t abs_flag=0x01;
	this->_mover->AllActuatorsMoveTo(abs_flag, &target_ik_a.alpha);
	if (true){
		Serial.print("\n    [Debug] CncSingleAxis::RunG1()     (");
		this->_mover->GetSingleActuatorCurrentPosition_InCncUnit(this->__AXIS_NAME);
		Serial.print(")   <-- from   alpha   to -->  (");
		Serial.print(target_ik_a.alpha  );
		// Serial.print(" , ");
		// Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}


float CncSingleAxis::GetDistanceToTarget_IK(){
	this->_mover->GetAbsDistanceToTarget_InCncUnit();

}


