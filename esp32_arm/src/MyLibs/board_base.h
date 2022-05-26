#pragma once

#include <HardwareSerial.h>
// #ifdef BOARD_WIRE
    #include <Wire.h>
// #endif

#include <Adafruit_MCP23X17.h>

class BoardBase{
    public:
        void ScanI2cBus(TwoWire* i2c_bus, const char* printing_topic);

    protected:
        TwoWire* _Make_I2cBus(uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency);
        Adafruit_MCP23X17* _Make_Mcp23018(uint8_t i2c_address, TwoWire* i2c_bus);

    private:
        uint8_t __i2c_bus_index = 0;
};



class BoardbaseCnc: protected BoardBase{
    public:
        virtual void EnableMotor_alpha(bool enable_it);
        virtual void EnableMotor_beta(bool enable_it);
        
    protected:

    private:

};

