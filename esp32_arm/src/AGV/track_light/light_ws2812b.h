#pragma once


#include "light_base.h"
#include <Adafruit_NeoPixel.h>

class Light_WS2812B: public LightBase{
    public:
        Light_WS2812B(uint16_t ledWs2812B_counts);
        void LinkDriver(Adafruit_NeoPixel* pixels){this->__pixels=pixels;};
        void TurnOn(bool turn_on) override;

    private:
        Adafruit_NeoPixel* __pixels;
        uint16_t __LedWs2812B_counts;

};

