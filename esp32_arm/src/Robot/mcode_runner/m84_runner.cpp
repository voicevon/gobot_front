
#include "m84_runner.h"
#include "CNC/Actuator/actuator_list.h"

bool M84_Runner::StartToRun(Gcode* mcode){
    //TODO: CNC_AXIS_COUNT_IK,   vs CNC_AXIS_COUNT_FK
	for (int axis=0; axis<Actuator_List::Instance().GetItemsCount(); axis++){
		ActuatorBase* actuator = Actuator_List::Instance().GetActuator(axis);
		actuator->ForceStop();   //TODO:  Disable
	}
	return true;
}
