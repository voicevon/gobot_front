#include "single_x_a_arm_solution.h"

void SpringMaker_ArmSolution::IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	to_ik->Positions[AXIS_ALPHA] = from_fk->X;
}

void SpringMaker_ArmSolution::FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	to_fk->X = from_ik->Positions[AXIS_ALPHA];
}

