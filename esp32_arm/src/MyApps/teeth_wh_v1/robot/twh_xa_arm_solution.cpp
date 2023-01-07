#include "twh_xa_arm_solution.h"



void Twh1_XaArmSolution::real_MK_to_FK(MiddleKinematic* mk, FKPosition_XYZRPW* fk){
	fk->Y = mk->Arm_shaft_At_Y + __config.arm_length * cosf(mk->Arm_Angle);
	fk->X = __config.arm_length * sinf(mk->Arm_Angle);

	// Logger::Debug("Twh1_XaArmSolution::real_MK_to_FK()"`);
	// Logger::Print("fk.X", fk->X);
	// Logger::Print("fk.Y", fk->Y);
}

void Twh1_XaArmSolution::real_FK_to_MK(FKPosition_XYZRPW* fk, MiddleKinematic* mk){
	mk->Arm_Angle = asinf(fk->X / __config.arm_length);  // range should be in range of degree [-90, +90]
	mk->Arm_shaft_At_Y = fk->Y - __config.arm_length * cosf(mk->Arm_Angle);
	// Logger::Print("input.X", fk->X);
	// Logger::Print("arm_length", __config.arm_length);
	// Logger::Print("mk.Arm_Angle in deg", mk->Arm_Angle * RAD_TO_DEG);
	// Logger::Print("mk.arm_shart_at_Y", mk->Arm_shaft_At_Y);
}

void Twh1_XaArmSolution::real_MK_to_IK(MiddleKinematic* mk, IKPosition_abgdekl* ik){
	ik->Positions[AXIS_ALPHA] = mk->Arm_Angle *__config.arm_slope_steps_per_rad + mk->Arm_shaft_At_Y  * __config.linear_slope_steps_per_mm  ;   
	ik->Positions[AXIS_BETA] = mk->Arm_Angle * __config.arm_slope_steps_per_rad - mk->Arm_shaft_At_Y * __config.linear_slope_steps_per_mm;
	// Logger::Debug("Twh1_XaArmSolution::real_MK_to_IK()");
	// Logger::Print("__config.linear_slope_steps_per_mm", __config.linear_slope_steps_per_mm);
	// Logger::Print("__config.arm_slope_steps_per_rad", __config.arm_slope_steps_per_rad);
	// Logger::Print("Twh1_XaArmSolution::real_MK_to_IK()  alpha", ik->Positions[AXIS_ALPHA]);
	// Logger::Print("Twh1_XaArmSolution::real_MK_to_IK()  beta", ik->Positions[AXIS_BETA]);
}

void Twh1_XaArmSolution::real_IK_to_MK(IKPosition_abgdekl* ik, MiddleKinematic* mk){
	mk->Arm_shaft_At_Y = (ik->Positions[AXIS_ALPHA] - ik->Positions[AXIS_BETA]) / __config.linear_slope_steps_per_mm / 2;
	mk->Arm_Angle = (ik->Positions[AXIS_ALPHA] + ik->Positions[AXIS_BETA]) / __config.arm_slope_steps_per_rad / 2;  // Notice: angle has no range now.
	
	// Logger::Print("Twh1_XaArmSolution::real_IK_to_MK() mk.y", mk->Arm_shaft_At_Y);
	// Logger::Print("Twh1_XaArmSolution::real_IK_to_MK() mk.Angle in deg", mk->Arm_Angle * RAD_TO_DEG);
}

void Twh1_XaArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	// Logger::Info("Twh1_XaArmSolution::IK()");
	MiddleKinematic mk;
	real_FK_to_MK(from_fk, &mk);
	real_MK_to_IK(&mk, to_ik);
	to_ik->Positions[AXIS_GAMMA] =  from_fk->Z / __config.servo_slope - __config.servo_offset;
}

void Twh1_XaArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	// Logger::Debug("Twh1_XaArmSolution::FK()");
	MiddleKinematic mk;
	real_IK_to_MK(from_ik,&mk);
	real_MK_to_FK(&mk, to_fk);
	to_fk->Z = (from_ik->Positions[AXIS_GAMMA] + __config.servo_offset) * __config.servo_slope;
}

void Twh1_XaArmSolution::MK_to_FK(FKPosition_XYZRPW* from_mk, FKPosition_XYZRPW* to_fk){
	MiddleKinematic mk;
	mk.Arm_shaft_At_Y = from_mk->Y;
	mk.Arm_Angle = from_mk->Roll;
	real_MK_to_FK(&mk, to_fk);
}
void Twh1_XaArmSolution::MK_to_Ik(FKPosition_XYZRPW* from_mk, IKPosition_abgdekl* to_ik){
	MiddleKinematic mk;
	mk.Arm_shaft_At_Y = from_mk->Y;
	mk.Arm_Angle = from_mk->Roll;
	real_MK_to_IK(&mk, to_ik);

}

void Twh1_XaArmSolution::Test_FK_MK_FK(FKPosition_XYZRPW* input){
	MiddleKinematic mk;
	FKPosition_XYZRPW output;
	// Logger::Info("Twh1_XaArmSolution::Test_FK_MK_FK()");
	real_FK_to_MK(input, &mk);
	real_MK_to_FK(&mk, &output);
}


void Twh1_XaArmSolution::Test_IK_MK_IK(IKPosition_abgdekl* input){
	MiddleKinematic mk;
	IKPosition_abgdekl output;
	// Logger::Info("Twh1_XaArmSolution::Test_IK_MK_IK()");
	real_IK_to_MK(input, &mk);
	real_MK_to_IK(&mk, &output);
}

void Twh1_XaArmSolution::Test_MK_IK_MK(MiddleKinematic* input){
	IKPosition_abgdekl ik;
	MiddleKinematic output;
	// Logger::Info("Twh1_XaArmSolution::Test_MK_IK_MK()");
	real_MK_to_IK(input, &ik);
	real_IK_to_MK(&ik, &output);
}

void Twh1_XaArmSolution::Test_MK_FK_MK(MiddleKinematic* input){
	FKPosition_XYZRPW fk;
	MiddleKinematic output;
	// Logger::Info("Twh1_XaArmSolution::Test_MK_IK_MK()");
	real_MK_to_FK(input, &fk);
	real_FK_to_MK(&fk, &output);
}
