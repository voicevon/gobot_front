#include<math.h>
#include "gobot_chessboard_hw.h"
#include<Arduino.h>


GobotChessboardHardware::GobotChessboardHardware(){
  // this->axis_alpha = &obj_axis_alpha;
  // this->axis_beta = &obj_axis_beta;
}

void GobotChessboardHardware::HomeSingleAxis(char axis){ 
  while(!objHomeHelper_alpha.IsTriged()){
    objStepper_alpha.setTargetRel(100);
    objStepControl.move(objStepper_alpha);
  }
  // objAxis_Alpha.SetCurrentPosition(HOME_POSITION_ALPHA);

  while(!objHomeHelper_beta.IsTriged()){
    objStepper_beta.setTargetRel(100);
    objStepControl.move(objStepper_beta);
  }
  this->commuDevice->OutputMessage("Home is done.....");
}

void GobotChessboardHardware::_running_G28(){

}

// void GobotChessboardHardware::__HomeSpin(Stepper* homing_stepper, uint8_t home_pin ){
// }

/*
https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
*/
// IkPositionBase GobotChessboardHardware::IK(FkPosXY* fk){
//   IkPosXY ret;   //is risk here?
//   float rr1= (fk.x + LINK_0) * (fk.x + LINK_0) + fk.y * fk.y;
  
//   // float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
//   float r1 = sqrtf(rr1);
//   float alpha_eef = acosf((fk.x + LINK_0) / r1);
//   float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B) / ( 2*LINK_A * r1));
//   float alpha = alpha_eef + alpha_link;
//   ret.alpha = alpha * STEPS_PER_RAD;

//   float rr2 = (fk.x - LINK_0)* (fk.x - LINK_0) + fk.y * fk.y;
//   float r2 = sqrtf(rr2);
//   float beta_eef = acosf((fk.x - LINK_0) / r2 );
//   float beta_link = acosf((LINK_A * LINK_A + rr2 - LINK_B * LINK_B) / (2 * LINK_A * r2));
//   float beta = beta_eef - beta_link;
//   ret.beta =  beta * STEPS_PER_RAD; 
//   return ret;
// }

void GobotChessboardHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
  FkPosition_XY* fk = (FkPosition_XY*)(from_fk);
  IkPosition_AB* ik = (IkPosition_AB*)(to_ik);

  float rr1= (fk->X + this->__config.LINK_0) * (fk->X + this->__config.LINK_0) + fk->Y * fk->Y;
  
  float r1 = sqrtf(rr1);
  float alpha_eef = acosf((fk->X + this->__config.LINK_0) / r1);
  float alpha_link = acosf((this->__config.LINK_A * this->__config.LINK_A + rr1 - this->__config.LINK_B * this->__config.LINK_B) / ( 2*this->__config.LINK_A * r1));
  float alpha = alpha_eef + alpha_link;
  ik->alpha  = alpha * this->__config.steps_per_rad;

  float rr2 = (fk->X - this->__config.LINK_0)* (fk->X - this->__config.LINK_0) + fk->Y * fk->Y;
  float r2 = sqrtf(rr2);
  float beta_eef = acosf((fk->X - this->__config.LINK_0) / r2 );
  float beta_link = acosf((this->__config.LINK_A * this->__config.LINK_A + rr2 - this->__config.LINK_B *this->__config. LINK_B) / (2 * this->__config.LINK_A * r2));
  float beta = beta_eef - beta_link;
  ik->beta =  beta * this->__config.steps_per_rad; 
}

void GobotChessboardHardware::FK(IkPositionBase* from_ik, FkPositionBase* to_fk){

}

void GobotChessboardHardware::SetEffector(EEF action){
  switch (action){
    case Lower:
      eefServo->write(180);
      break;
    case Higher:
      eefServo->write(0);
      break;
    case Suck:
      digitalWrite(PIN_EEF_A, HIGH);
      digitalWrite(PIN_EEF_B, LOW);
      break;
    case Release:
      digitalWrite(PIN_EEF_A,LOW);
      digitalWrite(PIN_EEF_B,HIGH);
      break;
    case Sleep:
      digitalWrite(PIN_EEF_A,LOW);
      digitalWrite(PIN_EEF_B,LOW);
      break;
    default:
      break;

  }
}

void GobotChessboardHardware::pick_place_park(RobotAction* pAction){
  uint8_t action_code = pAction->Arm.action_code;
    // for(int i=0;i<14;i++){
    //   Serial.print(pAction->bytes[i]);
    //   Serial.print(",  ");
    // }

  if ((action_code & (1<<1)) > 0){
    Serial.print("\npicking up "); 
    // MoveTo(pAction->Arm.pickup_x, pAction->Arm.pickup_y);
    SetEffector(Lower);
    SetEffector(Suck);
    SetEffector(Higher);
  }
  if ((action_code & (1<<2)) > 0){
    Serial.print("\n  placing     ");
    // for(int i=5;i<9;i++){
    //   Serial.print(pAction->bytes[i]);
    //   Serial.print(",  ");
    // }    
    // MoveTo(pAction->Arm.place_x, pAction->Arm.place_y);
    SetEffector(Lower);
    SetEffector(Release);
    SetEffector(Higher);
    SetEffector(Sleep);
  }
  if ((action_code & (1<<3)) > 0){
    Serial.print("\n  parking     ");
    // MoveTo(pAction->Arm.park_x, pAction->Arm.park_y);
    SetEffector(Sleep);
  }
  // pAction->bytes[0] = 1;
}

void GobotChessboardHardware::Setup(RobotAction* pAction){
  __arm_action = pAction;
  // Serial.print("Arm is Initializing.........");
  // __Mcp23018 = &Mcp23018::getInstance();
  Servo sv = Servo();
  sv.attach(PIN_EEF_SERVO);
  eefServo = &sv ;


  pinMode(PIN_EEF_A, OUTPUT);
  pinMode(PIN_EEF_B,OUTPUT);
  SetEffector(Sleep);
  // With libery AccelStepper
  // AccelStepper stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
  //                                     PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  // stepper_alpha = & stepper;
  // steppers.addStepper(stepper);
  // stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
  //                       PIN_BETA_STEP,PIN_BETA_DIR);
  // stepper_beta = & stepper;
  // steppers.addStepper(stepper);


  // With liberary ESP32Step
  // stepper_alpha = new Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  // stepper_beta = new Stepper(PIN_BETA_STEP, PIN_BETA_DIR);

  // stepper_alpha->setMaxSpeed(MOTOR_MAX_SPEED);
  // stepper_beta->setMaxSpeed(MOTOR_MAX_SPEED);
  digitalWrite(PIN_BETA_ENABLE, LOW);

  // this->__config. = LINK_0;
  // link_a = LINK_A;
  // link_b = LINK_B;
}


void GobotChessboardHardware::Init(){
    pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    pinMode(PIN_BETA_ENABLE, OUTPUT);
    digitalWrite(PIN_ALPHA_ENABLE, LOW);
    digitalWrite(PIN_BETA_ENABLE, LOW);

    this->commuDevice = &this->objCommuUart;
    // this->objAxis_Alpha.LinkAcuator(&this->objActuator_Alpha);
    // this->objActuator_Alpha.LinkDriver(nullptr);
    // this->objActuator_Alpha.LinkSensorHelper(nullptr);
    // this->objAxis_Beta.LinkAcuator(&this->objActuator_Beta);
    // this->objActuator_Beta.LinkDriver(nullptr);
    // this->objActuator_Beta.LinkSensorHelper(nullptr);

    // this->objHomeHelper_alpha.LinkAxis(&this->objAxis_Alpha);
    // this->objHomeHelper_alpha.LinkActuator(&this->objActuator_Alpha);
    // this->objHomeHelper_beta.LinkAxis(&this->objAxis_Beta);
    // this->objHomeHelper_beta.LinkActuator(&this->objActuator_Beta);
} 

void GobotChessboardHardware::RunG1(Gcode* gcode){
  
}

void GobotChessboardHardware::_running_G1(){
  
}