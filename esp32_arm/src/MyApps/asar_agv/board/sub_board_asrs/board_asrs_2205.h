#pragma once
 
#include "MyLibs/board/board_base.h"
#include "ASRS/sensor_jetty/sharp_ir_on_ads1115.h"
#include "../board_pins/board_pins_ver_2_0.h"

class BoardAsrs: public BoardBase{
    public:
        BoardAsrs(){};
        void Init();
        void LinkTwoWireBus(TwoWire* i2c_bus);
        JettySensor_SharpIrOnAds1115* GetJettySensor(){return &this->__jettySensor;};

    private:
        JettySensor_SharpIrOnAds1115 __jettySensor;
        Adafruit_ADS1115 __ads1115;
};