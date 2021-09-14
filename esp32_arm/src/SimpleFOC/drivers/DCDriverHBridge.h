#ifndef DCDriverHBridge_h
#define DCDriverHBridge_h

#include "../common/base_classes/DCDriver.h"
#include "../common/foc_utils.h"
#include "../common/time_utils.h"
#include "../common/defaults.h"
#include "hardware_api.h"

/**
 3 pwm bldc driver class
*/
class DCDriverHBridge: public DCDriver
{
  public:
    /**
      DCDriver class constructor
      @param pinA A phase pwm pin
      @param pinB B phase pwm pin
    */
    DCDriverHBridge(int pinA,int pinB);
    
    /**  Motor hardware init function */
  	int init() override;
    /** Motor disable function */
  	// void disable() override;
    // /** Motor enable function */
    // void enable() override;

    // hardware variables
  	int __pinA; //!< phase A pwm pin number
  	int __pinB; //!< phase B pwm pin number

    /** 
     * Set phase voltages to the harware 
     * 
     * @param Ua - phase A voltage
     * @param Ub - phase B voltage
     * @param Uc - phase C voltage
    */
    // void setPwm(float U_coil) override;

    /** 
     * Set phase voltages to the harware 
     * 
     * @param sc - phase A state : active / disabled ( high impedance )
     * @param sb - phase B state : active / disabled ( high impedance )
     * @param sa - phase C state : active / disabled ( high impedance )
    */
    // virtual void setPhaseState(int sa, int sb, int sc)  override;
  private:
        
};


#endif
