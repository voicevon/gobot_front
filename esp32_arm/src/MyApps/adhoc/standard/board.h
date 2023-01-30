#pragma once
#include "MyLibs/basic/board_base.h"
#include "MyLibs/component/led_gpio.h"

#define PIN_STATE_LED 2


class Adhoc_RouterBoard: public BoardBase{
    public:
        void Init() override;
        Led_GPIO* GetStateLed(){return &__rx_led;};
        
    private:
        void __InitHardware();
        Led_GPIO __rx_led;

};