#pragma once

// #include "../common/base_classes/DCDriver.h"
// #include "../common/foc_utils.h"
// #include "../common/time_utils.h"
// #include "../common/defaults.h"
// #include "hardware_api.h"
#include "../DriverBase.h"

/**
 DC motor H-Bridge driver class
*/
class DCDriverHBridge: public DriverBase{
  public:
    // DCDriverHBridge(){};
    virtual void Start(int pwm_speed, bool dir_forward) override;
    virtual void Stop() override;
    virtual void Break() override;
  
  protected:
       
  private:

        
};


