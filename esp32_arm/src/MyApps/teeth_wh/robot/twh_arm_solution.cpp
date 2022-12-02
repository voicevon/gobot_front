#include "twh_arm_solution.h"



void Twh_ArmSolution::real_MK_to_FK(MiddleKinematic* mk, FKPosition_XYZRPY* fk){
	Logger::Debug("Twh_ArmSolution::real_MK_to_FK()");
	fk->Y = mk->Arm_shaft_At_Y + __config.arm_length * cosf(mk->Arm_Angle);
	fk->X = __config.arm_length * sinf(mk->Arm_Angle);
	Logger::Print("fk.X", fk->X);
	Logger::Print("fk.Y", fk->Y);
}

void Twh_ArmSolution::real_FK_to_MK(FKPosition_XYZRPY* fk, MiddleKinematic* mk){
	mk->Arm_Angle = asinf(fk->X / __config.arm_length);  // range should be in range of degree [-90, +90]
	mk->Arm_shaft_At_Y = fk->Y - __config.arm_length * cosf(mk->Arm_Angle);
	Logger::Print("input.X", fk->X);
	Logger::Print("arm_length", __config.arm_length);
	Logger::Print("mk.Arm_Angle in deg", mk->Arm_Angle * RAD_TO_DEG);
	Logger::Print("mk.arm_shart_at_Y", mk->Arm_shaft_At_Y);
}

void Twh_ArmSolution::real_MK_to_IK(MiddleKinematic* mk, IKPosition_abgdekl* ik){
	Logger::Debug("Twh_ArmSolution::real_MK_to_IK()");
	ik->Positions[AXIS_ALPHA] = - (mk->Arm_shaft_At_Y - __config.arm_length) * __config.linear_slope_steps_per_mm + mk->Arm_Angle *__config.arm_slope_steps_per_rad ;   
	ik->Positions[AXIS_BETA] = (mk->Arm_shaft_At_Y - __config.arm_length ) * __config.linear_slope_steps_per_mm + mk->Arm_Angle * __config.arm_slope_steps_per_rad;
	Logger::Print("Twh_ArmSolution::real_MK_to_IK()  alpha", ik->Positions[AXIS_ALPHA]);
	Logger::Print("Twh_ArmSolution::real_MK_to_IK()  beta", ik->Positions[AXIS_BETA]);
}

void Twh_ArmSolution::real_IK_to_MK(IKPosition_abgdekl* ik, MiddleKinematic* mk){
	// mk.X = (ik->alpha   - ik->beta) * __config.master_slope_steps_per_mm / 2;
	mk->Arm_shaft_At_Y = (ik->Positions[AXIS_BETA] - ik->Positions[AXIS_ALPHA]) / __config.linear_slope_steps_per_mm / 2 + __config.arm_length;
	// mk.Angle = (ik->alpha + ik->beta) / 2 / __config.slave_gear_circle_length;  // Notice: angle has no range now.
	mk->Arm_Angle = (ik->Positions[AXIS_ALPHA] + ik->Positions[AXIS_BETA]) / __config.arm_slope_steps_per_rad /2;  // Notice: angle has no range now.
	
	Logger::Print("Twh_ArmSolution::real_IK_to_MK() mk.y", mk->Arm_shaft_At_Y);
	Logger::Print("Twh_ArmSolution::real_IK_to_MK() mk.Angle in deg", mk->Arm_Angle * RAD_TO_DEG);

}


void Twh_ArmSolution::IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	Logger::Info("Twh_ArmSolution::IK()");
	MiddleKinematic mk;
	real_FK_to_MK(from_fk, &mk);
	real_MK_to_IK(&mk, to_ik);
	to_ik->Positions[AXIS_GAMMA] = from_fk->Z;   // TODO: through mk?
}

void Twh_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	// Logger::Debug("Twh_ArmSolution::FK()");
	MiddleKinematic mk;
	real_IK_to_MK(from_ik,&mk);
	real_MK_to_FK(&mk, to_fk);
	to_fk->Z = from_ik->Positions[AXIS_GAMMA];
}

void Twh_ArmSolution::MK_to_FK(FKPosition_XYZRPY* from_mk, FKPosition_XYZRPY* to_fk){
	MiddleKinematic mk;
	mk.Arm_shaft_At_Y = from_mk->Y;
	mk.Arm_Angle = from_mk->Roll;
	real_MK_to_FK(&mk, to_fk);
}
void Twh_ArmSolution::MK_to_Ik(FKPosition_XYZRPY* from_mk, IKPosition_abgdekl* to_ik){
	MiddleKinematic mk;
	mk.Arm_shaft_At_Y = from_mk->Y;
	mk.Arm_Angle = from_mk->Roll;
	real_MK_to_IK(&mk, to_ik);

}

void Twh_ArmSolution::Test_FK_MK_FK(FKPosition_XYZRPY* input){
	MiddleKinematic mk;
	FKPosition_XYZRPY output;
	Logger::Info("Twh_ArmSolution::Test_FK_MK_FK()");
	real_FK_to_MK(input, &mk);
	real_MK_to_FK(&mk, &output);
}


void Twh_ArmSolution::Test_IK_MK_IK(IKPosition_abgdekl* input){
	MiddleKinematic mk;
	IKPosition_abgdekl output;
	Logger::Info("Twh_ArmSolution::Test_IK_MK_IK()");
	real_IK_to_MK(input, &mk);
	real_MK_to_IK(&mk, &output);
}

void Twh_ArmSolution::Test_MK_IK_MK(MiddleKinematic* input){
	IKPosition_abgdekl ik;
	MiddleKinematic output;
	Logger::Info("Twh_ArmSolution::Test_MK_IK_MK()");
	real_MK_to_IK(input, &ik);
	real_IK_to_MK(&ik, &output);
}

