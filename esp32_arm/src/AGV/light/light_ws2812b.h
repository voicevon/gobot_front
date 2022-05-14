#pragma once


#include "light_base.h"
#include <Adafruit_NeoPixel.h>

class Light_WS2812B: public LightBase{
    public:
        void TurnOn(bool turnOn) override;

    private:
        Adafruit_NeoPixel* pixels;

};

