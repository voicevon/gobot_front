#pragma once


#include "light_base.h"
#include <Adafruit_NeoPixel.h>

class Light_WS2812B: public LightBase{
    public:
        Light_WS2812B(uint16_t ledWs2812B_counts, int16_t pin_WS2812_LED);
        void TurnOn(bool turn_on) override;

    private:
        Adafruit_NeoPixel* pixels;
        uint16_t __LedWs2812B_counts;

};

