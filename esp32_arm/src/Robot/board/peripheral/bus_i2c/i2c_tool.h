
#pragma once


#include <Wire.h>

class I2C_Tool{
    public:
        // static uint8_t Assign_i2c_bus_id(){return __i2c_bus_index; __i2c_bus_index++;};
        void ScanI2cBus(TwoWire* i2c_bus, const char* printing_topic);
        bool _Begin_I2cBus(TwoWire* i2c_bus, uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency);
    private:

};