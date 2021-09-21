#include<math.h>
#include "gobot_chessboard.h"
#include<Arduino.h>


GobotChessboard::GobotChessboard(){
  // this->axis_alpha = &obj_axis_alpha;
  // this->axis_beta = &obj_axis_beta;
}

void GobotChessboard::HomeAllAxises(){ 
  while(!objHomeHelper_alpha.IsTriged()){
    objStepper_alpha.setTargetRel(100);
    objStepControl.move(objStepper_alpha);
  }
  objAxis_Alpha.SetCurrentPosition(ALPHA_HOME_POSITION_2112);

  while(!objHomeHelper_beta.IsTriged()){
    objStepper_beta.setTargetRel(100);
    objStepControl.move(objStepper_beta);
  }
  objAxis_Beta.SetCurrentPosition(BETA_HOME_POSITION_2112);
  this->commuDevice->OutputMessage("Home is done.....");
}

void GobotChessboard::__HomeSpin(Stepper* homing_stepper, uint8_t home_pin ){
  // uint8_t flags=0;
  // // Serial.print("\nHome spin got started.............\n");
  // while (flags !=0xff){
  //   stepper_alpha->setTargetRel(1);
  //   steppers->move(*homing_stepper);
  //   flags <<= 1;
  //   flags |= digitalRead(home_pin);
  // }
  // Serial.print("\n###################  Home is done.");
}

/*
https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
*/
ik_position GobotChessboard::ik(float x, float y){
  ik_position ret;   //is risk here?
  float rr1= (x + LINK_0_2112) * (x + LINK_0_2112) + y * y;
  
  // float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
  float r1 = sqrtf(rr1);
  float alpha_eef = acosf((x + LINK_0_2112) / r1);
  float alpha_link = acosf((LINK_A_2112 * LINK_A_2112 + rr1 - LINK_B_2112 * LINK_B_2112) / ( 2*LINK_A_2112 * r1));
  float alpha = alpha_eef + alpha_link;
  ret.alpha = alpha * STEPS_PER_RAD;

  float rr2 = (x - LINK_0_2112)* (x - LINK_0_2112) + y * y;
  float r2 = sqrtf(rr2);
  float beta_eef = acosf((x - LINK_0_2112) / r2 );
  float beta_link = acosf((LINK_A_2112 * LINK_A_2112 + rr2 - LINK_B_2112 * LINK_B_2112) / (2 * LINK_A_2112 * r2));
  float beta = beta_eef - beta_link;
  ret.beta =  beta * STEPS_PER_RAD; 
  return ret;
}

void GobotChessboard::SetEffector(EEF action){
  switch (action){
    case Lower:
      eefServo->write(180);
      break;
    case Higher:
      eefServo->write(0);
      break;
    case Suck:
      digitalWrite(PIN_EEF_A_2112, HIGH);
      digitalWrite(PIN_EEF_B_2112, LOW);
      break;
    case Release:
      digitalWrite(PIN_EEF_A_2112,LOW);
      digitalWrite(PIN_EEF_B_2112,HIGH);
      break;
    case Sleep:
      digitalWrite(PIN_EEF_A_2112,LOW);
      digitalWrite(PIN_EEF_B_2112,LOW);
      break;
    default:
      break;

  }
}

void GobotChessboard::pick_place_park(RobotAction* pAction){
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

void GobotChessboard::Setup(RobotAction* pAction){
  __arm_action = pAction;
  // Serial.print("Arm is Initializing.........");
  // __Mcp23018 = &Mcp23018::getInstance();
  Servo sv = Servo();
  sv.attach(PIN_EEF_SERVO_2112);
  eefServo = &sv ;


  pinMode(PIN_EEF_A_2112, OUTPUT);
  pinMode(PIN_EEF_B_2112,OUTPUT);
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
  digitalWrite(PIN_BETA_ENABLE_2112, LOW);

  link_0 = LINK_0_2112;
  link_a = LINK_A_2112;
  link_b = LINK_B_2112;
}

void GobotChessboard::SpinOnce(){
  this->_base_spin_once();
}


void GobotChessboard::Init(){
    pinMode(PIN_ALPHA_ENABLE_2112, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2112, OUTPUT);
    digitalWrite(PIN_ALPHA_ENABLE_2112, LOW);
    digitalWrite(PIN_BETA_ENABLE_2112, LOW);

    this->commuDevice = &this->objCommuUart;
    this->objAxis_Alpha.LinkAcuator(&this->objActuator_Alpha);
    this->objActuator_Alpha.linkDriver(nullptr);
    this->objActuator_Alpha.linkSensor(nullptr);
    this->objAxis_Beta.LinkAcuator(&this->objActuator_Beta);
    this->objActuator_Beta.linkDriver(nullptr);
    this->objActuator_Beta.linkSensor(nullptr);

    this->objHomeHelper_alpha.LinkAxis(&this->objAxis_Alpha);
    this->objHomeHelper_beta.LinkAxis(&this->objAxis_Beta);
} 

void GobotChessboard::RunG1(Gcode* gcode){
  
}