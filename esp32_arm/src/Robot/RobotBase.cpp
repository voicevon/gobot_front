#include "RobotBase.h"
#include "MyLibs/MyFunctions.hpp"


void RobotBase::SpinOnce(){
  commuDevice->SpinOnce();
  this->SpinOnce_BaseEnter();
  if(commuDevice->HasNewChatting()){
    Serial.println ("    _base_spin_once()  new chatting");
    std::string command(commuDevice->ReadChatting());
    Serial.println(command.c_str());
    Gcode gCode = Gcode(command);   //Risk for not releasing memory ?
    this->RunGcode(&gCode);
  }
  this->SpinOnce_BaseExit();
}

void RobotBase::RunGcode(Gcode* gcode){
  if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND)){
    Serial.print("RunGcode()   OK or Unknown");
    return;
  }

  if(!gcode->has_g){
    this->commuDevice->OutputMessage("  Has NO letter 'G'.");
    this->commuDevice->OutputMessage(gcode->get_command());
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
    return;
  }



  if (gcode->g == 28){
    // G28: Home
    // this->__is_busy = true;
    this->HomeAllAxises();
    // this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'

  }else if (gcode->g ==1){
    // G1 Move
    // this->__is_busy = true;
    //TODO:  1. put position to movement queue. called "plan" in smoothieware? 
    //       2. send out OK.
    //       3. Set status to busy.
    //       4. Start Moving.
    this->RunG1(gcode);
    this->commuDevice->OutputMessage(COMMU_OK);
  }else{
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
  }

}


