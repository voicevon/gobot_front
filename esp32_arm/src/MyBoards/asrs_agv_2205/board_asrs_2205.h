#pragma once

#include "board_pins_ver_2_0.h"
#include "ASRS/sensor_jetty/sharp_ir_on_ads1115.h"
#include "MyBoards/board_base.h"

class BoardAsrs: public BoardBase{
    public:
        BoardAsrs(){};
        void Init(bool is_on_reset);
        void LinkTwoWireBus(TwoWire* i2c_bus);
        JettySensor_SharpIrOnAds1115* GetJettySensor(){return &this->__jettySensor;};

    private:
        JettySensor_SharpIrOnAds1115 __jettySensor;
        Adafruit_ADS1115 __ads1115;
};