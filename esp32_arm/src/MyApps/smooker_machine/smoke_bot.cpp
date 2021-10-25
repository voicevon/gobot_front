#include "smoke_bot.h"

SmokeBot::SmokeBot(){

}

void SmokeBot::RunG1(Gcode* gcode) {
  //None blocking, move backgroundly.
  Serial.println("---------  Running G1   ");
  float distance = gcode->get_value('X');
  
  Serial.print("  ");
  Serial.print(distance);

  this->objStepper.setTargetAbs(distance);
  this->objStepControl.move(this->objStepper);
  // this->objStepControl.move(this->objStepper);
}

void SmokeBot::HomeAllAxises(){
  while (!this->objHomeHelper.IsTriged()){
    this->objStepper.setTargetRel(-50);
    this->objStepControl.move(this->objStepper);
  }
  this->objStepper.setPosition(0);
  Serial.print(" Homed postion =    " );
  Serial.print(this->objStepper.getPosition());
}

void SmokeBot::Init_Gpio(){
  Serial.println("Smoke bot is initializing GPIO ...");
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(MICRO_STEP_1, OUTPUT);
  pinMode(MICRO_STEP_2, OUTPUT);
  pinMode(MICRO_STEP_3, OUTPUT);

  digitalWrite(PIN_ENABLE, LOW);
  pinMode(MICRO_STEP_1, HIGH);
  pinMode(MICRO_STEP_2, HIGH);
  pinMode(MICRO_STEP_3, HIGH);


  this->objStepper.setAcceleration(25000);
  this->objStepper.setMaxSpeed(1000);
  this->objStepper.setStepPinPolarity(LOW);
}

void SmokeBot::Init_Linkage(){
  Init_Gpio();
  this->commuDevice = &this->objCommuUart; 
  // this->objHomeHelper.LinkAxis(&this->objAxis_Alpha);
  this->objHomeHelper.LinkActuator(&this->objActuator);

  // this->objAxis_Alpha.LinkAcuator(&this->objActuator_Alpha);
  // this->objActuator_Alpha.LinkDriver(nullptr);
  // this->objActuator_Alpha.LinkSensorHelper(nullptr);
  this->objCommuUart.OutputMessage("Hoddy, I am commmunicator UART ....");

}