// #include "box_mover_hw.h"
#include "core_az_arm_solution.h"

// #define PIN_ALPHA_ENABLE 18
// #define PIN_BETA_ENABLE 16
// #define PIN_MICRIO_STEP_2 21
// #define PIN_MICRIO_STEP_1 22
// #define PIN_MICRIO_STEP_0 23

// #define STEPS_PER_RAD 123   //2048 / 2*Pi
// #define STEPS_PER_MM 345   //2048 / 2*Pi




void CncSolution_CoreAZ::IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	Serial.print("\n[Info] CncSolution_CoreAZ::IK() is entering. ");
	FKPosition_XYZRPY* fk = (FKPosition_XYZRPY*)(from_fk);
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(to_ik);

	ik->alpha = (fk->Z  + fk->Roll);
	ik->beta = (fk->Z  - fk->Roll );

	Serial.print("\n[Debug] CncSolution_CoreAZ::IK() output (alpha, beta) = ");
	Serial.print(ik->alpha);
	Serial.print(" , ");
	Serial.print(ik->beta);
	Serial.print(")");
}

void CncSolution_CoreAZ::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	Serial.print("\n[Debug] CncSolution_CoreAZ::FK() is entering ");
	FKPosition_XYZRPY* fk = (FKPosition_XYZRPY*)(to_fk);
	IKPosition_abgdekl* ik =from_ik;
	
	fk->Z = (ik->alpha + ik->beta) / 2 ;
	fk->Roll = (ik->alpha - ik->beta) / 2 ;

	Serial.print("\n[Debug] CncSolution_CoreAZ::FK() output (Z, W) = ");
	Serial.print(fk->Z);
	Serial.print(" , ");
	Serial.print(fk->Roll);
	Serial.print(")");
}


// void CncSolution_CoreAZ::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
// //Set current position to HomePosition
// 		IKPosition_abgdekl ik_position;
// 			// We know homed position via FK
// 			Logger::Info("CncSolution_CoreAZ::_SetCurrentPositionAsHome()  Trying to get home position with EEF FK position  ");
// 			this->__current_fk_position.Z = this->_config->Homed_position_z;
// 			this->__current_fk_position.Roll = this->_config->Homed_position_w;
// 			this->IK(&this->__current_fk_position, &ik_position);
// 			// verify IK by FK()
// 			FKPosition_XYZRPY verifying_fk;
// 			Serial.print("\n   [Info] Please verify: FK->IK->FK  ");
// 			this->FK(&ik_position, &verifying_fk);
		
// }



// void CncSolution_CoreAZ::RunG1(Gcode* gcode) {
// bool CncSolution_CoreAZ::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
// 	Serial.print("\n[Debug] CncSolution_CoreAZ::RunG1() is entering");
// 	Serial.print(gcode->get_command());
// 	// this->_cnc_board->EnableMotor(AXIS_ALPHA, true);
// 	// this->_cnc_board->EnableMotor(AXIS_BETA, true);
// 	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
// 	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
// 	if (gcode->has_letter('F')){
// 		float speed = gcode->get_value('F');
// 		// this->_mover_base->SetEefSpeed(speed);
// 		mb->MoveBlocks[AXIS_ALPHA].Speed = speed;
// 		mb->MoveBlocks[AXIS_BETA].Speed = speed;
// 	}
// 	// Assume G1-code want to update actuator directly, no need to do IK.
// 	FKPosition_XYZRPY target_fk_zw;
// 	IKPosition_abgdekl target_ik_ab;
// 	target_fk_zw.Z = this->__current_fk_position.Z;
// 	target_fk_zw.Roll = this->__current_fk_position.Roll;
// 	// target_ik_ab.alpha = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
// 	// target_ik_ab.beta = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
// 	bool do_ik=false;
// 	if (gcode->has_letter(AXIS_ALPHA)) target_ik_ab.alpha = gcode->get_value(AXIS_ALPHA);
// 	if (gcode->has_letter(AXIS_BETA)) target_ik_ab.beta = gcode->get_value(AXIS_BETA);

// 	// If need IK, do it now.
// 	if (gcode->has_letter('Z')) {
// 		do_ik=true;
// 		target_fk_zw.Z = gcode->get_value('Z');
// 	}
// 	if (gcode->has_letter('W')){
// 		do_ik=true;
// 		target_fk_zw.Roll = gcode->get_value('R');
// 	}
// 	if (do_ik) IK(&target_fk_zw,&target_ik_ab);

// 	//Prepare actuator/driver to move to next point
// 	// float motor_position[2];
// 	// motor_position[0] = target_ik_ab.alpha;
// 	// motor_position[1] = target_ik_ab.beta;
// 	mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_ab.alpha;
// 	mb->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_ab.beta;
// 	// this->__queue_move_block->ForwardHead();
// 	Queue_MoveBlock::Instance().Deposit();
// 	//None blocking, move backgroundly.
// 	// this->_mover_base->AllActuatorsMoveTo(true, motor_position);

// 	if (true){
// 		Serial.print("\n    [Debug] CncSolution_CoreAZ::RunG1()     (");
// 		// Serial.print(this->objStepper_alpha->getPosition());
// 		// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
// 		Serial.print(",");
// 		// Serial.print(this->objStepper_beta->getPosition());
// 		// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
// 		Serial.print(")   <-- from   alpha,beta   to -->  (");
// 		Serial.print(target_ik_ab.alpha  );
// 		Serial.print(" , ");
// 		Serial.print(target_ik_ab.beta);
// 		Serial.print(")");
// 	}
// }
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



// float CncSolution_CoreAZ::GetDistanceToTarget_IK(){
// 	// return this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA) + this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
// }


