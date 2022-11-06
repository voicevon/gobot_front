
#include "m84_runner.h"
#include "CNC/Actuator/cnc_actuator_list.h"

bool M84_Runner_Disable_All_Steppers::StartToRun(Gcode* mcode){
    //TODO: CNC_AXIS_COUNT_IK,   vs CNC_AXIS_COUNT_FK
	for (int axis=0; axis<CncActuator_List::Instance().GetItemsCount(); axis++){
		CncActuatorBase* actuator = CncActuator_List::Instance().GetActuator(axis);
		actuator->ForceStop();   //TODO:  Disable
	}
	return true;
}
