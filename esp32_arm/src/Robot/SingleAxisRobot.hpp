#ifndef _SINGLE_AXIS_HPP_
#define _SINGLE_AXIS_HPP_

#include "SingleAxisRobot.h"

template<class Actuator_T>
SingleAxisRobot<Actuator_T>::SingleAxisRobot(char axisName){
    this->_Axis_Name = axisName;
} 

template <class Actuator_T>
void SingleAxisRobot<Actuator_T>::RunGcode(Gcode* gcode){

  if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND))
    return;

  if(!gcode->has_g){
    this->__output_message(COMMU_UNKNOWN_COMMAND);
    return;
  }

  float code =  gcode->get_value('G');
  if (code == 28){
    // G28: Home
    this->__is_busy = true;
    this->Home();
    this->__output_message(COMMU_OK);

  }else if (code ==1){
    // Move
    this->__is_busy = true;
    float pos = gcode->get_value(this->_Axis_Name);

    //TODO:  1. put position to movement queue. called "plan" in smoothieware? 
    //       2. send out OK.
    //       3. Set status to busy.
    //       4. Start Moving.
    this->Move(pos);
    this->__output_message(COMMU_OK);
  }else{
    this->__output_message(COMMU_UNKNOWN_COMMAND);
  }

}

#endif