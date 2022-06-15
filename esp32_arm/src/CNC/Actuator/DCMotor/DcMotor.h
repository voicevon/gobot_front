#ifndef DCMotor_h
#define DCMotor_h

#include "Arduino.h"
// #include "../ActuatorBase.h"
#include "../ActuatorBase_Ver0.1.h"
// #include "SimpleFOC/common/base_classes/Sensor.h"
#include <SimpleFOC.h>
#include "DCDriverHBridge.h"


/**
  Essencially, DCMotor is a Name shelled class, The reasons of we keep this class are: 
      1.  A better name is easier to interpretered. 
      2.  In case of some identically property or method comes out in future.
*/
class DCMotor: public ActuatorBaseVer_0_1{
  public:
    // DCMotor(char axis_name){this->AxisName = axis_name;};
    // void linkDriver(DCDriverHBridge* driver);   //????
    // bool IsRunning(){return __is_running;};
    
  private:
    // DCDriverHBridge* driver; 
    // bool __is_running;
        
};


#endif
