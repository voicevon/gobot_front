#include "RobotArm.h"

void RobotArm::LinkStepper(Stepper* alpha, Stepper* beta){
  stepper_alpha = alpha;
  stepper_beta = beta;
}

void RobotArm::MoveTo(int16_t x, int16_t y){
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(" (x,y)  <<<<   >>>> (alpah,beta)   ");

  motor_position pos = ik(x,y);

  if (pos.alpha ==-99999){
    Serial.println("IK return -99999");
    return;
  }

  Serial.print(pos.alpha);
  Serial.print(", ");
  Serial.print(pos.beta);

  stepper_alpha->setTargetAbs(pos.alpha);
  stepper_beta->setTargetAbs(pos.beta);
  steppers->move(*stepper_alpha, * stepper_beta);
}