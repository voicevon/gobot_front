#include "core_yz_arm_solution.h"


void CoreYZ_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	Serial.print("\n[Info] CoreYZ_ArmSolution::IK() is entering. ");
	FKPosition_XYZRPW * fk = (FKPosition_XYZRPW*)(from_fk);
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(to_ik);

	// ik->alpha = (fk->Z  + fk->Y );
	ik->Positions[AXIS_ALPHA] = (fk->Z  + fk->Y );
	// ik->beta = (fk->Z - fk->Y );
	ik->Positions[AXIS_BETA] = (fk->Z - fk->Y );

	Serial.print("\n[Debug] CoreYZ_ArmSolution::IK() output (alpha, beta) = ");
	// Serial.print(ik->alpha);
	Serial.print(ik->Positions[AXIS_ALPHA]);
	Serial.print(" , ");
	// Serial.print(ik->beta);
	Serial.print(ik->Positions[AXIS_BETA]);
	Serial.print(")");
}

void CoreYZ_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	Serial.print("\n[Debug] CoreYZ_ArmSolution::FK() is entering ");
	FKPosition_XYZRPW* fk = (FKPosition_XYZRPW*)(to_fk);
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(from_ik);
	
	// fk->Z = (ik->alpha + ik->beta) / 2;
	fk->Z = (ik->Positions[AXIS_ALPHA] + ik->Positions[AXIS_BETA]) / 2;
	// fk->Y = (ik->alpha - ik->beta) / 2;
	fk->Y = (ik->Positions[AXIS_ALPHA] - ik->Positions[AXIS_BETA]) / 2;

	Serial.print("\n[Debug] CoreYZ_ArmSolution::FK() output (Z, Y) = ");
	Serial.print(fk->Z);
	Serial.print(" , ");
	Serial.print(fk->Y);
	Serial.print(")");
}


// void CoreYZ_ArmSolution::Init(CncBoardBase* board){
// 	Serial.print("\n[Info] CoreYZ_ArmSolution::Init_Linkage() is entering.");
// 	// this->_cncMachine = (CncSolution_CoreYZConfigBase*)(this->_board->GetCncConfig());
// 	this->_board = board;

// 	this->objHomeHelper_y = board->GetHomer(AXIS_Y);
// 	this->objHomeHelper_vertical = board->GetHomer(AXIS_Z);


// }

// void CoreYZ_ArmSolution::RunG28_CombinedAxis(EnumAxis_ForwardKinematic axis){
// 	Serial.print("[Debug] CoreYZ_ArmSolution::RunG28() is entering:   " );
// 	Serial.println(axis);
// 	// this->_homing_axis = axis;
// 	this->_cncMachine->PrintOut("CoreYZ_ArmSolution::RunG28()");
// 	// Serial.println(this->stepper_alpha->getPosition());
// 	// this->stepper_alpha->setAcceleration(this->_cncMachine->Homing_acceleration_alpha_beta);
// 	// this->stepper_alpha->setMaxSpeed(this->_cncMachine->Homing_speed_alpha_beta);
// 	// this->stepper_beta->setAcceleration(this->_cncMachine->Homing_acceleration_alpha_beta);
// 	// this->stepper_beta->setMaxSpeed(this->_cncMachine->Homing_speed_alpha_beta);
// 	// float motor_position[2];
// 	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
// 	MoveBlock* mb = gs_MoveBlock_Queue::Instance().GetRoom_ForDeposit();
// 	mb->MoveBlocks[AXIS_ALPHA].IsAbsTargetPosition = false;
// 	mb->MoveBlocks[AXIS_BETA].IsAbsTargetPosition = false;
// 	if (axis=='Y'){
// 		//todo :  process with IK()
// 		this->__homing_helper = this->objHomeHelper_y;
// 		// this->stepper_alpha->setTargetRel(-5000000);
// 		// this->stepper_beta->setTargetRel(5000000);
// 		// motor_position[0] = -5000000;
// 		// motor_position[1] = 5000000;
// 		mb->MoveBlocks[AXIS_ALPHA].TargetPosition = -5000000;
// 		mb->MoveBlocks[AXIS_BETA].TargetPosition = 5000000;

// 	}else if (axis=='Z'){
// 		this->__homing_helper = this->objHomeHelper_vertical;
// 		// this->stepper_alpha->setTargetRel(-5000000);
// 		// this->stepper_beta->setTargetRel(-5000000);
// 		// motor_position[0]=-5000000;
// 		// motor_position[1]=-5000000;	
// 		mb->MoveBlocks[AXIS_ALPHA].TargetPosition = -5000000;
// 		mb->MoveBlocks[AXIS_BETA].TargetPosition = -5000000;
// 	}

	
// 	// this->_mover_base->AllActuatorsMoveTo(false, motor_position);
// 	// this->__queue_move_block->ForwardHead();	
// 	gs_MoveBlock_Queue::Instance().Deposit();

// 	// this->_board->EnableMotor('A', true);
// 	// this->_board->EnableMotor('B',true);

// 	// this->_stepControl->moveAsync(*this->stepper_alpha, *this->stepper_beta);
// }

// void CoreYZ_ArmSolution::_SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis){
// 		//Set current position to HomePosition
// 		IKPosition_abgdekl ik_position;
// 			// We know homed position via FK
// 			Logger::Info("CoreYZ_ArmSolution::_SetCurrentPositionAsHome() Trying to get home position with EEF FK position  ");
// 			this->__current_fk_position.Z = this->_cncMachine->Homed_position_z;
// 			this->__current_fk_position.Y = this->_cncMachine->Homed_position_y;
// 			this->IK(&this->__current_fk_position, &ik_position);
// 			// verify IK by FK()
// 			FKPosition_XYZRPW verifying_fk;
// 			Serial.print("\n   [Info] Please verify: FK->IK->FK ======================  ");
// 			this->FK(&ik_position, &verifying_fk);
// }


// bool CoreYZ_ArmSolution::_CutGcodeLine_ToSegmentQueue(GcodeText* gcode){
// 	Serial.print("\n[Debug] CoreYZ_ArmSolution::RunG1() is entering");
// 	Serial.print(gcode->get_command());
// 	// this->_cnc_board->EnableMotor(AXIS_ALPHA, true);
// 	// this->_cnc_board->EnableMotor(AXIS_BETA, true);
// 	// MoveBlock* mb = this->__queue_move_block->GetHeadMoveblock();
// 	MoveBlock* mb = gs_MoveBlock_Queue::Instance().GetRoom_ForDeposit();
// 	if (gcode->has_letter('F')){
// 		float speed = gcode->get_value('F');
// 		// this->stepper_alpha->setMaxSpeed(speed);
// 		// this->stepper_beta->setMaxSpeed(speed);
// 		// this->_mover_base->SetEefSpeed(speed);
// 		mb->MoveBlocks[AXIS_ALPHA].Speed = speed;
// 		mb->MoveBlocks[AXIS_BETA].Speed = speed;
// 	}
// 	// Assume G1-code want to update actuator directly, no need to do IK.
// 	FKPosition_XYZRPW target_fk_yz;
// 	IKPosition_abgdekl target_ik_ab;
// 	target_fk_yz.Z = this->__current_fk_position.Z;
// 	target_fk_yz.Y = this->__current_fk_position.Y;
// 	// target_ik_ab.alpha = float(this->stepper_alpha->getPosition()) ;
// 	// target_ik_ab.beta = float(this->stepper_beta->getPosition());
// 	bool do_ik=false;
// 	if (gcode->has_letter('A')) target_ik_ab.alpha = gcode->get_value('A');
// 	if (gcode->has_letter('B')) target_ik_ab.beta = gcode->get_value('B');

// 	// If need IK, do it now.
// 	if (gcode->has_letter('Z')) {
// 		do_ik=true;
// 		target_fk_yz.Z = gcode->get_value('Z');
// 	}
// 	if (gcode->has_letter('Y')){
// 		do_ik=true;
// 		target_fk_yz.Y = gcode->get_value('Y');
// 	}
// 	if (do_ik) IK(&target_fk_yz,&target_ik_ab);

// 	//Prepare actuator/driver to move to next point
// 	// this->stepper_alpha->setTargetAbs(target_ik_ab.alpha);
// 	// this->stepper_beta->setTargetAbs(target_ik_ab.beta);
// 	// //None blocking, move backgroundly.
// 	// this->_stepControl->moveAsync(*this->stepper_alpha, *this->stepper_beta);
// 	// float target_motor_position[2];
// 	// target_motor_position[0] = target_ik_ab.alpha;
// 	// target_motor_position[1] = target_ik_ab.beta;
// 	// this->_mover_base->AllActuatorsMoveTo(true, target_motor_position);

// 	mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_ab.alpha;
// 	mb->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_ab.beta;
// 	// this->__queue_move_block->ForwardHead();
// 	gs_MoveBlock_Queue::Instance().Deposit();

// 	if (true){
// 		Serial.print("\n    [Debug] CoreYZ_ArmSolution::RunG1()     (");
// 		// Serial.print(this->stepper_alpha->getPosition());
// 		Serial.print(",");
// 		// Serial.print(this->stepper_beta->getPosition());
// 		Serial.print(")   <-- from   alpha,beta   to -->  (");
// 		Serial.print(target_ik_ab.alpha  );
// 		Serial.print(" , ");
// 		Serial.print(target_ik_ab.beta);
// 		Serial.print(")");
// 	}
// }



// float CoreYZ_ArmSolution::GetDistanceToTarget_IK(){
// 	// return this->stepper_alpha->getDistanceToTarget() + this->stepper_beta->getDistanceToTarget();
// 	// return this->_mover_base->GetAbsDistanceToTarget_InCncUnit();
// }


// void CoreYZ_ArmSolution::__ConvertSegment_ToMoveBlockQueue(LineSegment* line){

// }

