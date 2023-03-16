#pragma once

#include "MyLibs/component/remote_base/remotable_slave_base.h"
#include <Adafruit_NeoPixel.h>


class WS2812B: public RemoteabeSlaveBase{
    public:
        void Init();
        void Init_Remotable(const char* mqtt_topic);


    private:
        Adafruit_NeoPixel __leds;


};