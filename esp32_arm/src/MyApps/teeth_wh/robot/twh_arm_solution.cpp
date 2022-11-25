#include "twh_arm_solution.h"

class MiddleKinematic{
	public:
		float X;
		float Angle;
		// float Minimuim_X;
};

void Twh_ArmSolution::IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	Serial.print("\n[Info] Twh_ArmSolution::IK()");
	MiddleKinematic mk;
	FKPosition_XYZRPY* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	mk.Angle = asinf(fk->Y / _config->arm_length);  // range should be in range of degree [-90, +90]
	mk.X = fk->X - _config->arm_length * cosf(mk.Angle);
	if (mk.X < _config->middle_kinematic_minimuim_X){
		// solution of angle in range of degree [-180,-90]  or [90-180]
		mk.X += _config->arm_length * cosf(mk.Angle) * 2.0f;
		if(mk.Angle > 0){
			mk.Angle = PI - mk.Angle;
		}else{
			mk.Angle = -PI - mk.Angle;
		}
	}


	// ik->alpha = (mk.X + mk.Angle /_config->slave_gear_circle_length ) / _config->master_slope_steps_per_mm * 2;   
	ik->Actuator[AXIS_ALPHA] = (mk.X + mk.Angle /_config->slave_gear_circle_length ) / _config->master_slope_steps_per_mm * 2;   
	// ik->beta = (mk.X - mk.Angle / _config->slave_gear_circle_length) / _config->master_slope_steps_per_mm * 2;
	ik->Actuator[AXIS_BETA] = (mk.X - mk.Angle / _config->slave_gear_circle_length) / _config->master_slope_steps_per_mm * 2;

	// Serial.print("\n[Debug] Twh_ArmSolution::IK() output (alpha, beta) = ");
	// Serial.print(ik->alpha);
	// Serial.print(" , ");
	// Serial.print(ik->beta);
	// Serial.print(")");
}

void Twh_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	Serial.print("\n[Debug] Twh_ArmSolution::FK()");
	FKPosition_XYZRPY* fk = (FKPosition_XYZRPY*)(to_fk);
	IKPosition_abgdekl* ik = (IKPosition_abgdekl*)(from_ik);
	
	MiddleKinematic mk;

	// mk.X = (ik->alpha   - ik->beta) * _config->master_slope_steps_per_mm / 2;
	mk.X = (ik->Actuator[AXIS_ALPHA]   - ik->Actuator[AXIS_BETA]) * _config->master_slope_steps_per_mm / 2;
	// mk.Angle = (ik->alpha + ik->beta) / 2 / _config->slave_gear_circle_length;  // Notice: angle has no range now.
	mk.Angle = (ik->Actuator[AXIS_ALPHA] + ik->Actuator[AXIS_BETA]) / 2 / _config->slave_gear_circle_length;  // Notice: angle has no range now.

	fk->X = mk.X + _config->arm_length * cosf(mk.Angle);
	fk->Y = _config->arm_length * sinf(mk.Angle);

	// fk->X = (ik->alpha + ik->beta) / 2 ;
	// fk->Y = (ik->alpha - ik->beta) / 2 ;

	// Serial.print("\n[Debug] Twh_ArmSolution::FK() output (Z, W) = ");
	// Serial.print(fk->X);
	// Serial.print(" , ");
	// Serial.print(fk->Y);
	// Serial.print(")");
}



