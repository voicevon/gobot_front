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
  const char* result;
  if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND)){
    Serial.print("RunGcode()   OK or Unknown");
    return;
  }

  if(gcode->has_g){
    switch (gcode->g){
      case 28:
        // G28: Home
        // this->__is_busy = true;
        this->HomeAllAxises();
        // this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'
        break;
      case 1:
        // G1 Move
        // this->__is_busy = true;
        //TODO:  1. put position to movement queue. called "plan" in smoothieware? 
        //       2. send out OK.
        //       3. Set status to busy.
        //       4. Start Moving.
        this->RunG1(gcode);
        this->commuDevice->OutputMessage(COMMU_OK);
        break;
      case 6:
        this->RunG6(gcode);
        this->commuDevice->OutputMessage(COMMU_OK);
        break;
      case 90:
        // Absolute position
        this->is_absolute_position = true;
        break;
      case 91:
        // Relative position
        this->is_absolute_position = false;
        break;
      case 92:
        // Set Position     G92 X10 E90
        break;
      default:
        break;
    }
  }else if(gcode->has_m){
    switch (gcode->m){
      case 119:
        // Get Endstop Status
        result = GetHomeTrigerStateString();
        Serial.print(result);
        this->commuDevice->OutputMessage(result);
        break;
      case 114:
        // Get Current Position
        break;
      default:
        break;
    }
  }else{
    this->commuDevice->OutputMessage("  Has NO letter 'G' or 'M'. ");
    this->commuDevice->OutputMessage(gcode->get_command());
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
  }
}


