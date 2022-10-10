#include "core_xy_ab_arm_solution.h"

class MiddleKinematic{
	public:
		float X;
		float Angle;
		// float Minimuim_X;
};

void CncSolution_CoreXY_ab::IK(FkPositionBase* from_fk,IkPositionBase* to_ik){
	Serial.print("\n[Info] CncSolution_CoreXY_ab::IK()");
	MiddleKinematic mk;
	FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
	IkPosition_AlphaBeta* ik = (IkPosition_AlphaBeta*)(to_ik);

	mk.Angle = asinf(fk->Y / arm_length);  // range should be in range of degree [-90, +90]
	mk.X = fk->X - arm_length * cosf(mk.Angle);
	if (mk.X < middle_kinematic_minimuim_X){
		// solution of angle in range of degree [-180,-90]  or [90-180]
		mk.X += arm_length * cosf(mk.Angle) * 2.0f;
		if(mk.Angle > 0){
			mk.Angle = PI - mk.Angle;
		}else{
			mk.Angle = -PI - mk.Angle;
		}
	}


	ik->alpha = (mk.X + mk.Angle /slave_gear_circle_length ) / master_slope_steps_per_mm * 2;   
	ik->beta = (mk.X - mk.Angle / slave_gear_circle_length) / master_slope_steps_per_mm * 2;

	// Serial.print("\n[Debug] CncSolution_CoreXY_ab::IK() output (alpha, beta) = ");
	// Serial.print(ik->alpha);
	// Serial.print(" , ");
	// Serial.print(ik->beta);
	// Serial.print(")");
}

void CncSolution_CoreXY_ab::FK(IkPositionBase* from_ik, FkPositionBase*  to_fk){
	Serial.print("\n[Debug] CncSolution_CoreXY_ab::FK()");
	FkPosition_XY* fk = (FkPosition_XY*)(to_fk);
	IkPosition_AlphaBeta* ik = (IkPosition_AlphaBeta*)(from_ik);
	
	MiddleKinematic mk;


	mk.X = (ik->alpha   - ik->beta) * master_slope_steps_per_mm / 2;
	mk.Angle = (ik->alpha + ik->beta) / 2 / slave_gear_circle_length;  // Notice: angle has no range now.

	fk->X = mk.X + arm_length * cosf(mk.Angle);
	fk->Y = arm_length * sinf(mk.Angle);

	// fk->X = (ik->alpha + ik->beta) / 2 ;
	// fk->Y = (ik->alpha - ik->beta) / 2 ;

	// Serial.print("\n[Debug] CncSolution_CoreXY_ab::FK() output (Z, W) = ");
	// Serial.print(fk->X);
	// Serial.print(" , ");
	// Serial.print(fk->Y);
	// Serial.print(")");
}

bool CncSolution_CoreXY_ab::_CutGcodeLine_ToSegmentQueue(Gcode* gcode){
	Serial.print("\n[Debug] CncSolution_CoreXY_ab::RunG1() is entering");
	Serial.print(gcode->get_command());
	MoveBlock* mb = Queue_MoveBlock::Instance().GetRoom();
	if (gcode->has_letter('F')){
		float speed = gcode->get_value('F');
		mb->MoveBlocks[AXIS_ALPHA].Speed = speed;
		mb->MoveBlocks[AXIS_BETA].Speed = speed;
	}
	// Assume G1-code want to update actuator directly, no need to do IK.
	FkPosition_XY FkPosition_XY;
	IkPosition_AlphaBeta target_ik_ab;
	FkPosition_XY.X = this->__current_fk_position.X;
	FkPosition_XY.Y = this->__current_fk_position.Y;
	// target_ik_ab.alpha = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA);
	// target_ik_ab.beta = this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA);
	bool do_ik=false;
	if (gcode->has_letter(AXIS_ALPHA)) target_ik_ab.alpha = gcode->get_value(AXIS_ALPHA);
	if (gcode->has_letter(AXIS_BETA)) target_ik_ab.beta = gcode->get_value(AXIS_BETA);

	// If need IK, do it now.
	if (gcode->has_letter('X')) {
		do_ik=true;
		FkPosition_XY.X = gcode->get_value('X');
	}
	if (gcode->has_letter('Y')){
		do_ik=true;
		FkPosition_XY.Y = gcode->get_value('Y');
	}
	if (do_ik) IK(&FkPosition_XY,&target_ik_ab);

	//Prepare actuator/driver to move to next point
	// float motor_position[2];
	// motor_position[0] = target_ik_ab.alpha;
	// motor_position[1] = target_ik_ab.beta;
	mb->MoveBlocks[AXIS_ALPHA].TargetPosition = target_ik_ab.alpha;
	mb->MoveBlocks[AXIS_BETA].TargetPosition = target_ik_ab.beta;
	// this->__queue_move_block->ForwardHead();
	Queue_MoveBlock::Instance().Deposit();
	//None blocking, move backgroundly.

	if (true){
		Serial.print("\n    [Debug] CncSolution_CoreXY_ab::RunG1()     (");
		// Serial.print(this->objStepper_alpha->getPosition());
		// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_ALPHA));
		Serial.print(",");
		// Serial.print(this->objStepper_beta->getPosition());
		// Serial.print(this->_mover_base->GetSingleActuatorCurrentPosition_InCncUnit(AXIS_BETA));
		Serial.print(")   <-- from   alpha,beta   to -->  (");
		Serial.print(target_ik_ab.alpha  );
		Serial.print(" , ");
		Serial.print(target_ik_ab.beta);
		Serial.print(")");
	}
}


