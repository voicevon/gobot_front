#include "ActuatorBase.h"

/**
 * Default constructor - setting all variabels to default values
 */
ActuatorBase::ActuatorBase()
{
  sensor = nullptr;
}


void ActuatorBase::linkSensor(Sensor* _sensor) {
  sensor = _sensor;
}
