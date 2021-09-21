#ifndef DCDriverHBridge_h
#define DCDriverHBridge_h

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
    DCDriverHBridge(int pinA,int pinB);
    void Start(int pwm_speed, bool dir_forward);
    void Stop();
    void Break();
  
  private:
  	int __pinA; //!< phase A pwm pin number
  	int __pinB; //!< phase B pwm pin number
        
};


#endif
