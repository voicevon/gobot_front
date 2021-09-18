#ifndef DCMotor_h
#define DCMotor_h

#include "Arduino.h"
#include "MotorBase.h"
// #include "common/base_classes/FOCMotor.h"
// #include "common/base_classes/Sensor.h"
#include "SimpleFOC/common/base_classes/Sensor.h"
// #include "Common/base_classes/MotorBase.h"
#include "MotorBase.h"
// #include "common/base_classes/DCDriver.h"
// #include "drivers/DCDriverHBridge.h"
#include "DCDriverHBridge.h"

// #include "common/foc_utils.h"
// #include "common/time_utils.h"
// #include "common/defaults.h"

/**
 DC motor class
*/
class DCMotor: public MotorBase{
  public:
    DCMotor();
    
    void linkDriver(DCDriverHBridge* driver);
    void init() override;
    bool IsRunning(){return __is_running;};
  	// void disable() override;
    // void enable() override;

    // int initFOC( float zero_electric_offset = NOT_SET , Direction sensor_direction = Direction::CW) override;
    // void loopFOC() override;

    void move(float target = NOT_SET) override;
    
  private:
    DCDriverHBridge* driver; 
    bool __is_running;
        
};


#endif
