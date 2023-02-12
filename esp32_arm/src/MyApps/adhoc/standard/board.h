#pragma once
#include "MyLibs/board/board_base.h"
#include "MyLibs/component/display/mono_led_gpio.h"

#define PIN_STATE_LED 2


class Adhoc_RouterBoard: public BoardBase{
    public:
        void Init() override;
        Mono_Led_GPIO* GetStateLed(){return &__rx_led;};
        
    private:
        void __InitHardware();
        Mono_Led_GPIO __rx_led;

};