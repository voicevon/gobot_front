#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/button_gpio.h"
#include "../ws2812b/ws2812b.h"

#define PIN_GPIO_BUTTON_PICKED 32   // green
#define PIN_GPIO_BUTTON_PACKED 33    // blue
#define PIN_NEO_PIXEL  26   // Arduino pin that connects to NeoPixel

class Twh_Packer_Board: public BoardBase{
    public:
        Twh_Packer_Board();
        void Init() override;
        Button_Gpio* GetButton_picked(){return &__picked_button;};
        Button_Gpio* GetButton_Packed(){return &__packed_button;};
        Adafruit_NeoPixel* GetNeoPixel(){return & __neo_pixel;};
        WS2812B* GetWs2812B(){return &__ws2812b;};
        void BootTest();
        // Adafruit_NeoPixel* GetLeds(){return &__leds;};

        // void TestButton(int test_loop_count);

    protected:

    private:
        Button_Gpio __picked_button = Button_Gpio(PIN_GPIO_BUTTON_PICKED);
        Button_Gpio __packed_button = Button_Gpio(PIN_GPIO_BUTTON_PACKED);
        WS2812B __ws2812b;
        Adafruit_NeoPixel __neo_pixel = Adafruit_NeoPixel(WS2812B_COUNT, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
        // void __SetLeds();
        // int __led_red_index = -1;
        // int __led_green_index = -1;
        // int __led_blue_index = -1;

};