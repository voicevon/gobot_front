#include "sillicon_pump_arm_solution.h"
#include "sillicon_pump_arm_solution_config.h"

Silicon_Pump_ArmSolution::Silicon_Pump_ArmSolution(){
	Silicon_Pump_Armsolution_Config config;
	__slope = config.Slope_Steps_per_box();
}
void Silicon_Pump_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPW* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	ik->Positions[AXIS_ALPHA] = fk->X * __slope ;
	ik->Positions[AXIS_BETA] = fk->Y * __slope;
	ik->Positions[AXIS_GAMMA] = fk->Z * __slope;
	Logger::Debug("Silicon_Pump_ArmSolution::IK()");
	// Logger::Print("Silicon_Pump_ArmSolution::FK_to_IK()  ")

	Logger::Print("IK output alpha", ik->Positions[AXIS_ALPHA]);
	Logger::Print("IK output beta", ik->Positions[AXIS_BETA]);
	Logger::Print("IK output gamma", ik->Positions[AXIS_GAMMA]);
}

void Silicon_Pump_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	FKPosition_XYZRPW* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	fk->X = ik->Positions[AXIS_ALPHA] / __slope;
	fk->Y = ik->Positions[AXIS_BETA] / __slope;
	fk->Z = ik->Positions[AXIS_GAMMA] / __slope;

	// Logger::Debug("Silicon_Pump_ArmSolution::FK()" );
	// Logger::Print("FK output X", fk->X);
}



