#include "ActuatorBase.h"

float ActuatorBase::GetCurrentPos(){
  return this->current_position;
}

void ActuatorBase::SetCurrentPos(float position){
  this->current_position = position;
}


