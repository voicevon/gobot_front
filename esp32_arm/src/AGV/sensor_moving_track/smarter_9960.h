#pragma once
#include <stdint.h>
#include "Adafruit_APDS9960.h"

class Smarter9960{
    public:
        Smarter9960(uint8_t i2c_bus_id, uint8_t pin_sda, uint8_t pin_sclk);
        Smarter9960(TwoWire* i2c_bus);
        uint16_t color_r, color_g, color_b, color_c;
        void ReadSensor();
        float light_percent;
    
    protected:
        void _Init(TwoWire* i2c_bus);
    private:
        Adafruit_APDS9960* sensor;
        float c_min, c_max;
        uint16_t c_min_idle_counter = 0;
        uint16_t c_max_idle_counter =0;
        float r_min, r_max, g_min, g_max, b_min, b_max;
};