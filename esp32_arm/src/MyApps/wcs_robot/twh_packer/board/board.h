#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/button_gpio.h"
#include "MyLibs/component/display/ws2812b/ws2812b.h"

#define PIN_NEO_PIXEL  22   // Arduino pin that connects to NeoPixel
#define PIN_GPIO_BUTTON_PICKED 19
#define PIN_GPIO_BUTTON_PACKED 21

// class Twh_Packer_Board: public BoardBase{
class Twh_Packer_Board: public BoardBase{
    public:
        Twh_Packer_Board();
        void Init() override;

        
        Button_Gpio* GetButton_picked(){return &__picked_button;};
        Button_Gpio* GetButton_Packed(){return &__packed_button;};
        WS2812B* GetLeds(){return & __leds;};
        // Adafruit_NeoPixel* GetLeds(){return &__leds;};

        void TestButton(int test_loop_count);

    protected:

    private:
        Button_Gpio __picked_button = Button_Gpio(PIN_GPIO_BUTTON_PICKED);
        Button_Gpio __packed_button = Button_Gpio(PIN_GPIO_BUTTON_PACKED);
        WS2812B __leds = WS2812B();
        // Adafruit_NeoPixel __leds = Adafruit_NeoPixel(WS2812B_COUNT, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
        // void __SetLeds();
        // int __led_red_index = -1;
        // int __led_green_index = -1;
        // int __led_blue_index = -1;

};