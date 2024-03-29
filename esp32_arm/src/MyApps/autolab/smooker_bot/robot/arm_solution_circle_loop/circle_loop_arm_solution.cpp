#include "circle_loop_arm_solution.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h"

void Smoker_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPW* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	// ik->alpha = fk->Roll;
	ik->Positions[AXIS_ALPHA] = fk->Roll;
	Logger::Debug("Smoker_ArmSolution::IK()");
	// Logger::Print("IK output alpha", ik->alpha);
	Logger::Print("IK output alpha", ik->Positions[AXIS_ALPHA]);
}

void Smoker_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	FKPosition_XYZRPW* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	// fk->Roll = ik->alpha;
	fk->Roll = ik->Positions[AXIS_ALPHA];
	Logger::Debug("Smoker_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->Roll);
}

