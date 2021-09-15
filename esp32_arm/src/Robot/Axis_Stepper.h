#ifndef _AXIS_STEPPER_H_
#define _AXIS_STEPPER_H_


#include "SingleAxisBase.h"
#include "ESP32Step/src/TeensyStep.h"

// #include "SimpleFOC/SimpleFOC.h"

class AxisStepper: public SingleAxisBase{
    public:
        AxisStepper();
        AxisStepper(uint8_t _id);
        Stepper* motor;
        
        void LinkMotor(Stepper* stepper);
        // DCMotor* motor;
        // Encoder* encoder;
        // DCDriver* driver;
        // void Init(DCDriverHBridge* driver, Encoder* encoder, uint8_t pin_home);
        // void Init(Ste);

    private:
        void __Init_motor();

};

#endif