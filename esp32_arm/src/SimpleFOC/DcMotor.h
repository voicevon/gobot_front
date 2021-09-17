#ifndef DCMotor_h
#define DCMotor_h

#include "Arduino.h"
#include "common/base_classes/FOCMotor.h"
#include "common/base_classes/Sensor.h"
// #include "common/base_classes/DCDriver.h"
#include "drivers/DCDriverHBridge.h"
#include "common/foc_utils.h"
#include "common/time_utils.h"
#include "common/defaults.h"

/**
 DC motor class
*/
class DCMotor: public FOCMotor{
  public:
    DCMotor();
    
    void linkDriver(DCDriverHBridge* driver);
    void init() override;
  	void disable() override;
    void enable() override;

    int initFOC( float zero_electric_offset = NOT_SET , Direction sensor_direction = Direction::CW) override;
    void loopFOC() override;

    void move(float target = NOT_SET) override;
    
  private:
    DCDriverHBridge* driver; 
        
};


#endif
