#pragma once
#include <stdint.h>
#include "Adafruit_APDS9960.h"


class SmartLightSensor{
    public:
        SmartLightSensor(uint8_t i2c_bus_id, uint8_t pin_sda, uint8_t pin_sclk);
        uint16_t color_r, color_g, color_b, color_c;
        void ReadSensor();
        float light_percent;
    private:
        // void Learn();
        Adafruit_APDS9960* sensor;
        // Adafruit_APDS9960* __apds_right;
        float c_min, c_max;
        float r_min, r_max, g_min, g_max, b_min, b_max;

};