#ifndef DCDriverHBridge_h
#define DCDriverHBridge_h

#include "../common/base_classes/DCDriver.h"
#include "../common/foc_utils.h"
#include "../common/time_utils.h"
#include "../common/defaults.h"
#include "hardware_api.h"

/**
 DC motor H-Bridge driver class
*/
class DCDriverHBridge: public DCDriver{
  public:
    /**
      DCDriverHBridge class constructor
      @param pinA A phase pwm pin
      @param pinB B phase pwm pin
    */
    DCDriverHBridge(int pinA,int pinB);
    
    /**  Motor hardware init function */
  	int init() override;

    // hardware variables
  	int __pinA; //!< phase A pwm pin number
  	int __pinB; //!< phase B pwm pin number

  private:
        
};


#endif
