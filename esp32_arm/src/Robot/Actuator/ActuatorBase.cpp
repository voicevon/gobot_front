#include "ActuatorBase.h"

float ActuatorBase::GetCurrentPos(){
  return this->sensorHelper.GetPosition();
}

void ActuatorBase::SetCurrentPos(float position){
  sensorHelper.SetCurrentAngleAs(position);
}


