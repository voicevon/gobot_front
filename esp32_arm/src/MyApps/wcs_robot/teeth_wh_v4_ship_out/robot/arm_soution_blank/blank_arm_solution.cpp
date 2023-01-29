#include "blank_arm_solution.h"

Blank_ArmSolution::Blank_ArmSolution(){
}

void Blank_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	Logger::Debug("Blank_ArmSolution::IK()");
}

void Blank_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	Logger::Debug("Blank_ArmSolution::FK()" );
}



