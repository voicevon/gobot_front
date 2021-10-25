#include "smoke_bot.h"

#define ACCELERATION 250000
#define MAX_SPEED 23000   // 1500 is OK
#define ACCELERATION_HOMIMG  250000
#define MAX_SPEED_HOMING  22000

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
  Serial.println("\n================================  " );
  Serial.print(" Start homing    " );
  this->objStepper.setAcceleration(ACCELERATION_HOMIMG);
  this->objStepper.setMaxSpeed(MAX_SPEED_HOMING);
  this->objStepper.setTargetRel(-50000);
  this->objStepControl.moveAsync(this->objStepper);
  while (! this->objHomeHelper.IsTriged()){
    // Serial.print(".");
    delay(10);
  }
  this->objStepControl.stop();
  this->objStepper.setPosition(0);
  this->objStepper.setAcceleration(ACCELERATION);
  this->objStepper.setMaxSpeed(MAX_SPEED);
  Serial.print(" Homed postion =    " );
  Serial.println(this->objStepper.getPosition());
}

void SmokeBot::Init_Gpio(){
  Serial.println("Smoke bot is initializing GPIO ...");
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(MICRO_STEP_1, OUTPUT);
  pinMode(MICRO_STEP_2, OUTPUT);
  pinMode(MICRO_STEP_3, OUTPUT);

  digitalWrite(PIN_ENABLE, LOW);
  digitalWrite(MICRO_STEP_1, HIGH);
  digitalWrite(MICRO_STEP_2, HIGH);
  digitalWrite(MICRO_STEP_3, HIGH);

  this->objStepper.setStepPinPolarity(HIGH);
}

void SmokeBot::Init_Linkage(){
  this->commuDevice = &this->objCommuUart; 
  // this->objHomeHelper.LinkAxis(&this->objAxis_Alpha);
  this->objHomeHelper.LinkActuator(&this->objActuator);

  // this->objAxis_Alpha.LinkAcuator(&this->objActuator_Alpha);
  // this->objActuator_Alpha.LinkDriver(nullptr);
  // this->objActuator_Alpha.LinkSensorHelper(nullptr);
  this->objCommuUart.OutputMessage("Hoddy, I am commmunicator UART ....");

}