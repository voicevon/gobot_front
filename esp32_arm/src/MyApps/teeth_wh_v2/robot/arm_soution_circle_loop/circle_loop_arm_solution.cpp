// #include "vsc_arm_solution.h"

// #include "vsc_arm_solution_config.h"

#include "circle_loop_arm_solution.h"
#include "circle_loop_arm_solution_config.h"
#include "Robot/axis_homer/position_trigger_array.h"

Twh2_Circleloop_ArmSolution::Twh2_Circleloop_ArmSolution(){
	Twh2_Circleloop_Armsolution_Config config;
	__sensor_slope = config.Slope_chain_rad_per_sensor_rad();
}
void Twh2_Circleloop_ArmSolution::FK_to_IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPY* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	ik->Positions[AXIS_ALPHA] = fk->Roll / __sensor_slope - __sensor_offset ;
	Logger::Debug("Twh2_Circleloop_ArmSolution::IK()");
	// Logger::Print("IK output alpha", ik->alpha);
	Logger::Print("IK output alpha", ik->Positions[AXIS_ALPHA]);
}

void Twh2_Circleloop_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk){
	FKPosition_XYZRPY* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	fk->Roll = __sensor_slope * (ik->Positions[AXIS_ALPHA] + __sensor_offset);
	Logger::Debug("Twh2_Circleloop_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->Roll);
}


