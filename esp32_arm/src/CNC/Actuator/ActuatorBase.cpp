#include "ActuatorBase.h"

float ActuatorBase::GetCurrentPos(){
  return this->sensorHelper->GetMeanValue();
}

void ActuatorBase::SetCurrentPos(float position){
  this->sensorHelper->SetCurrentMeanValueAs(position);
}

void ActuatorBase::SetTargetSpeed(float speed){
  this->TargetSpeed = speed;
}


