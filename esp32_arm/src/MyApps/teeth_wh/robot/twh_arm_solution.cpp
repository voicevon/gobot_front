#include "twh_arm_solution.h"


void Twh_ArmSolution::IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	Serial.print("\n[Info] Twh_ArmSolution::IK()");
	MiddleKinematic mk;
	FKPosition_XYZRPY* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	mk.Arm_Angle = asinf(fk->Y / _config->arm_length);  // range should be in range of degree [-90, +90]
	mk.Arm_shaft_At_Y = fk->X - _config->arm_length * cosf(mk.Arm_Angle);
	if (mk.Arm_shaft_At_Y < _config->middle_kinematic_minimuim_X){
		// solution of angle in range of degree [-180,-90]  or [90-180]
		mk.Arm_shaft_At_Y+= _config->arm_length * cosf(mk.Arm_Angle * 2.0f);
		if(mk.Arm_Angle > 0){
			mk.Arm_Angle = PI - mk.Arm_Angle;
		}else{
			mk.Arm_Angle = -PI - mk.Arm_Angle;
		}
	}


	// ik->alpha = (mk.X + mk.Angle /_config->slave_gear_circle_length ) / _config->master_slope_steps_per_mm * 2;   
	ik->Positions[AXIS_ALPHA] = (mk.Arm_shaft_At_Y + mk.Arm_Angle /_config->slave_gear_circle_length ) / _config->master_slope_steps_per_mm * 2;   
	// ik->beta = (mk.X - mk.Angle / _config->slave_gear_circle_length) / _config->master_slope_steps_per_mm * 2;
	ik->Positions[AXIS_BETA] = (mk.Arm_shaft_At_Y - mk.Arm_Angle / _config->slave_gear_circle_length) / _config->master_slope_steps_per_mm * 2;

	// Serial.print("\n[Debug] Twh_ArmSolution::IK() output (alpha, beta) = ");
	// Serial.print(ik->alpha);
	// Serial.print(" , ");
	// Serial.print(ik->beta);
	// Serial.print(")");
	ik->Positions[AXIS_GAMMA] = fk->Z;
}

void Twh_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	Serial.print("\n[Debug] Twh_ArmSolution::FK()");
	FKPosition_XYZRPY* fk = (FKPosition_XYZRPY*)(to_fk);
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(from_ik);
	
	MiddleKinematic mk;

	// mk.X = (ik->alpha   - ik->beta) * _config->master_slope_steps_per_mm / 2;
	mk.Arm_shaft_At_Y = (ik->Positions[AXIS_ALPHA]   - ik->Positions[AXIS_BETA]) * _config->master_slope_steps_per_mm / 2;
	// mk.Angle = (ik->alpha + ik->beta) / 2 / _config->slave_gear_circle_length;  // Notice: angle has no range now.
	mk.Arm_Angle = (ik->Positions[AXIS_ALPHA] + ik->Positions[AXIS_BETA]) / 2 / _config->slave_gear_circle_length;  // Notice: angle has no range now.

	fk->X = mk.Arm_shaft_At_Y + _config->arm_length * cosf(mk.Arm_Angle);
	fk->Y = _config->arm_length * sinf(mk.Arm_Angle);
	fk->Z = ik->Positions[AXIS_GAMMA];

	// fk->X = (ik->alpha + ik->beta) / 2 ;
	// fk->Y = (ik->alpha - ik->beta) / 2 ;

	// Serial.print("\n[Debug] Twh_ArmSolution::FK() output (Z, W) = ");
	// Serial.print(fk->X);
	// Serial.print(" , ");
	// Serial.print(fk->Y);
	// Serial.print(")");
}

void Twh_ArmSolution::MK_to_FK(FKPosition_XYZRPY* from_mk, FKPosition_XYZRPY* to_fk){

}
void Twh_ArmSolution::MK_to_Ik(FKPosition_XYZRPY* from_mk, IKPosition_abgdekl* to_ik){

}

void Twh_ArmSolution::real_MK_to_Fk(MiddleKinematic* mk, FKPosition_XYZRPY* fk){
	fk->X = mk->Arm_shaft_At_Y + _config->arm_length * cosf(mk->Arm_Angle);
	fk->Y = _config->arm_length * sinf(mk->Arm_Angle);
	// fk->Z = ik->Positions[AXIS_GAMMA];
}


