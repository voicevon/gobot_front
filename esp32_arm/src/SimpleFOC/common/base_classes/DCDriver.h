#ifndef DCDRIVER_H
#define DCDRIVER_H

#include "Arduino.h"

class DCDriver{
    public:
        
        /** Initialise hardware */
        virtual int init() = 0;
        /** Enable hardware */
        // virtual void enable() = 0;
        /** Disable hardware */
        // virtual void disable() = 0;

        long pwm_frequency; //!< pwm frequency value in hertz
        // float voltage_power_supply; //!< power supply voltage 
        // float voltage_limit; //!< limiting voltage set to the motor

        
        float duty_cycle; //!< currently set duty cycle on phaseA
            
        /** 
         * Set phase voltages to the harware 
         * 
         * @param UCoil - Output voltage
        */
        // virtual void setPwm(float UCoil) = 0;

        /** 
         * Set phase state, enable/disable 
         * 
         * @param sc - phase A state : active / disabled ( high impedance )
         * @param sb - phase B state : active / disabled ( high impedance )
         * @param sa - phase C state : active / disabled ( high impedance )
        */
        // virtual void setPhaseState(int sa, int sb, int sc) = 0;
};

#endif