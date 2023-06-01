#include "vsc_arm_solution.h"
#include "vsc_arm_solution_config.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h"

Vsc_ArmSolution::Vsc_ArmSolution(){
	Vsc_Armsolution_Config config;
	__sensor_slope = config.Slope_chain_rad_per_sensor_rad();
}
void Vsc_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPW* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	ik->Positions[AXIS_ALPHA] = fk->Roll / __sensor_slope - __sensor_offset ;
	Logger::Debug("Vsc_ArmSolution::IK()");
	// Logger::Print("IK output alpha", ik->alpha);
	Logger::Print("IK output alpha", ik->Positions[AXIS_ALPHA]);
}

void Vsc_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	FKPosition_XYZRPW* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	fk->Roll = __sensor_slope * (ik->Positions[AXIS_ALPHA] + __sensor_offset);
	Logger::Debug("Vsc_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->Roll);
}


