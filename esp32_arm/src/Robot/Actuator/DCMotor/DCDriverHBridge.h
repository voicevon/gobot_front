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
    DCDriverHBridge();
    virtual void Start(int pwm_speed, bool dir_forward);
    virtual void Stop();
    virtual void Break();
  
  protected:
    void _Init(int pinA,int pinB);
  	int _pinA; //!< phase A pwm pin number
  	int _pinB; //!< phase B pwm pin number 
       
  private:

        
};


#endif
