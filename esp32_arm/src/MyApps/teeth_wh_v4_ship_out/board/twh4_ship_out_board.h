#pragma once

#include "MyLibs/basic/board_base.h"
#include <Adafruit_NeoPixel.h>
#include"MyLibs/Components/gpio_button.h"

#define WS2812B_COUNT 24
#define PIN_NEO_PIXEL  22   // Arduino pin that connects to NeoPixel
#define PIN_GPIO_BUTTON 23

class Twh4_ShipOut_Board: public BoardBase{
    public:
        Twh4_ShipOut_Board();
        void Init() override;
        void SetLed(int position_index, int mode, int red, int green, int blue);  // 0= OFF, 1= ON, 2= FAST_FLASH, 3= SLOW_FLASH,  4= IN_TURN
        void ClearLeds();
        // void SpinOnce();
        GpioButton* GetButton(){return &__button;};

        void TestLed(int test_loop_count,int test_method, int red, int green, int blue);
        void TestButton(int test_loop_count);

    protected:

    private:
        Adafruit_NeoPixel __leds = Adafruit_NeoPixel(WS2812B_COUNT, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
        GpioButton __button = GpioButton(PIN_GPIO_BUTTON);

};