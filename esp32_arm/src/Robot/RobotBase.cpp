#include "RobotBase.h"
#include "MyLibs/MyFunctions.hpp"


void RobotBase::_base_spin_once(){
  commuDevice->SpinOnce();
  if(commuDevice->HasNewChatting()){
    std::string command(commuDevice->ReadChatting());
    // std::string command="G1 A12.345 ";
    commuDevice->OutputMessage(command);
    Gcode gCode = Gcode(command);
    // Gcode gCode = Gcode(commu.ReadChatting());   //Risk for not releasing memory ?
    this->RunGcode(&gCode);
  }
}

void RobotBase::RunGcode(Gcode* gcode){

  if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND))
    return;

  if(!gcode->has_g){
    this->commuDevice->OutputMessage("  Does NOT has letter 'G'.");
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
    return;
  }

  // float code =  gcode->get_value('G');
  
  if (gcode->g == 28){
    // G28: Home
    // this->__is_busy = true;
    this->HomeAllAxises();
    // this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'

  }else if (gcode->g ==1){
    // G1 Move
    this->RunG1(gcode);
    // this->__is_busy = true;
    // float pos = gcode->get_value(this->_Axis_Name);
    // this->commuDevice->OutputMessage("  > Move to " + toString(pos));
    //TODO:  1. put position to movement queue. called "plan" in smoothieware? 
    //       2. send out OK.
    //       3. Set status to busy.
    //       4. Start Moving.
    // this->SetTargetAbs(pos);
    // this->actuatorController.Move();
    // this->_actuator->SetTargetAbs(pos);
    // this->actuatorController->Move(relDistance);

    this->commuDevice->OutputMessage(COMMU_OK);
  }else{
    this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
  }

}

void RobotBase::AppendAxis(SingleAxis* axis){
  
}

