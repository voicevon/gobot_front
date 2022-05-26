#pragma once

#include <HardwareSerial.h>
#ifdef BOARD_WIRE
    #include <Wire.h>
#endif

#include <Adafruit_MCP23X17.h>

class PcbaBoardBase{
    public:
        TwoWire* MakeI2cBus(uint8_t pin_sda, uint8_t pin_scl, uint16_t frequency);
        void ScanI2cBus(TwoWire* i2c_bus);

        Adafruit_MCP23X17* MakeMcp23018(TwoWire* i2c_bus){};

    private:


};