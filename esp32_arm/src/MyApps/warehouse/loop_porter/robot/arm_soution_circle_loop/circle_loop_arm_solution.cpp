#include "circle_loop_arm_solution.h"
#include "circle_loop_arm_solution_config.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h"

Twh_Circleloop_ArmSolution::Twh_Circleloop_ArmSolution(){
	Twh_Circleloop_Armsolution_Config config;
	__slope = config.Slope_Steps_per_box();
}
void Twh_Circleloop_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPW* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	ik->Positions[AXIS_ALPHA] = fk->X * __slope ;
	// Logger::Debug("Twh_Circleloop_ArmSolution::IK()");
	// Logger::Print("IK output alpha", ik->Positions[AXIS_ALPHA]);
}

void Twh_Circleloop_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	FKPosition_XYZRPW* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	fk->X = ik->Positions[AXIS_ALPHA] / __slope;
	// Logger::Debug("Twh_Circleloop_ArmSolution::FK()" );
	// Logger::Print("FK output X", fk->X);
}



