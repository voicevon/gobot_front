#include "smoke_bot.h"

SmokeBot::SmokeBot(){

}

void SmokeBot::RunG1(Gcode* gcode) {
  //None blocking, move backgroundly.
}
void SmokeBot::Init_Linkage(){
//   init_gpio();
  this->commuDevice = &this->objCommuUart; 
  // this->objHomeHelper.LinkAxis(&this->objAxis_Alpha);
  this->objHomeHelper.LinkActuator(&this->objActuator);

  // this->objAxis_Alpha.LinkAcuator(&this->objActuator_Alpha);
  // this->objActuator_Alpha.LinkDriver(nullptr);
  // this->objActuator_Alpha.LinkSensorHelper(nullptr);
  this->objCommuUart.OutputMessage("Hoddy, I am commmunicator UART ....");

}