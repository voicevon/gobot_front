#pragma once

#include <Arduino.h>
#include "FunctionalInterrupt.h"

// http://www.gammon.com.au/forum/?id=12983     static instances code for attachInterupt()

class SingleAxis{
    public:
        SingleAxis(uint8_t pinA, uint8_t pinB, uint8_t pin_home_sensor);
        void SetTargetAbs(int targetPosition);
        void Setup(void (*ISR_callback)(void));

        void setup2(std::function<void(void)> callback);
        void ICACHE_RAM_ATTR ISR_Encoder(void);
        void(*ISR_callback)();
    private:
        // static void InteruptPinB();
        uint8_t __pinA; // Our first hardware interrupt pin is digital pin 2
        uint8_t __pinB; // Our second hardware interrupt pin is digital pin 3
        uint8_t __pinHomeSensor;
        // static volatile byte aFlag; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
        // static volatile byte bFlag; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
        volatile int32_t encoderPos ; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
        // static volatile byte oldEncPos ; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
        // static volatile byte reading ; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

};
