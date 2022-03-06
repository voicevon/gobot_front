#include "vehical_twin_wheel.h"

void VehicalTwinWheel::Init(){
	this->__commandQueue = new CommandQueue();
	this->__commandQueue->LinkRobot(&this->objTwinWheel_hardware);
}