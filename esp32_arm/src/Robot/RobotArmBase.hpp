#ifndef __ROBOTARM_BASE_H_
#define __ROBOTARM_BASE_H_

#include "RobotArmBase.h"

// void RobotArmBase::LinkStepper(Stepper* alpha, Stepper* beta){
//   stepper_alpha = alpha;
//   stepper_beta = beta;
// }
template<class Actuator_T, class ActuatorController_T>
void RobotArmBase<Actuator_T, ActuatorController_T>::MoveTo(int16_t x, int16_t y){
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(" (x,y)  <<<<   >>>> (alpah,beta)   ");

  ik_position pos = ik(x,y);

  if (pos.alpha ==-99999){
    Serial.println("IK return -99999");
    return;
  }

  Serial.print(pos.alpha);
  Serial.print(", ");
  Serial.print(pos.beta);
  axis_alpha->actuator->setTargetAbs(pos.alpha);
  axis_beta->actuator->setTargetAbs(pos.beta);
  actuatorController->move(*axis_alpha->actuator, *axis_beta->actuator);

  // stepper_alpha->setTargetAbs(pos.alpha);
  // stepper_beta->setTargetAbs(pos.beta);
  // steppers->move(*stepper_alpha, * stepper_beta);
}

template<class Actuator_T, class ActuatorController_T>
void RobotArmBase<Actuator_T, ActuatorController_T>::RunGcode(Gcode* gcode){
   if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND))
    return;

  if(!gcode->has_g){
    this->__output_message(COMMU_UNKNOWN_COMMAND);
    return;
  }
  float code =  gcode->get_value('G');
  if (code == 28){
    this->__is_busy = true;
    this->HomeAllAxises();
  }else if (code ==1){
    this->__is_busy = true;
    // float pos = gcode->get_value('X');
    // this->Move(pos);
  }else{
    this->__output_message(COMMU_UNKNOWN_COMMAND);
  }
  this->__output_message(COMMU_OK);
}



#endif