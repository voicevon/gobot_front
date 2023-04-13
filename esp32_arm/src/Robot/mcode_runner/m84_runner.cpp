
#include "m84_runner.h"
#include "CNC/Actuator/cnc_actuator_list.h"


void M84_Runner_Disable_All_Steppers::SetupRunner(GcodeText* mcode){
    
}

bool M84_Runner_Disable_All_Steppers::RunnerLoop(){
    //TODO: CNC_AXIS_COUNT_IK,   vs CNC_AXIS_COUNT_FK
	for (int axis=0; axis<gs_CncActuator_List::Instance().GetItemsCount(); axis++){
		CncActuatorBase* actuator = gs_CncActuator_List::Instance().GetActuator(axis);
		actuator->ForceStop();   //TODO:  Disable
	}
	return true;
}
