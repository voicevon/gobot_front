#ifndef _SINGLE_AXIS_HPP_
#define _SINGLE_AXIS_HPP_

#include "SingleAxis.h"

template<class Actuator_T>
SingleAxis<Actuator_T>::SingleAxis(char axisName){
    this->_Axis_Name = axisName;
} 

template <class Actuator_T>
void SingleAxis<Actuator_T>::RunGcode(Gcode* gcode){

  if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND))
    return;

  if(!gcode->has_g){
    this->__on_finished_gcode(COMMU_UNKNOWN_COMMAND);
    return;
  }
  float code =  gcode->get_value('G');
  if (code == 28){
    this->__is_busy = true;
    this->Home();
  }else if (code ==1){
    this->__is_busy = true;
    float pos = gcode->get_value('X');
    this->Move(pos);
  }else{
    this->__on_finished_gcode(COMMU_UNKNOWN_COMMAND);
  }
  this->__on_finished_gcode(COMMU_OK);

}

#endif