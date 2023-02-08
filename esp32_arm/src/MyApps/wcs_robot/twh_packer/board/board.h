#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/button_gpio.h"
#include <Adafruit_NeoPixel.h>

#define WS2812B_COUNT 24
#define PIN_NEO_PIXEL  22   // Arduino pin that connects to NeoPixel
#define PIN_GPIO_BUTTON_PICKED 19
#define PIN_GPIO_BUTTON_PACKED 20

// class Twh_Packer_Board: public BoardBase{
class Twh_Packer_Board: public BoardBase{
    public:
        Twh_Packer_Board();
        void Init() override;
        void SetLed_Red(int position_index); 
        void SetLed_Green(int position_index); 
        void SetLed_Blue(int position_index);  
        
        Button_Gpio* GetButton_picked(){return &__picked_button;};
        Button_Gpio* GetButton_Packed(){return &__packed_button;};

        void TestLed(int test_loop_count,int test_method, int red, int green, int blue);
        void TestButton(int test_loop_count);

    protected:

    private:
        Button_Gpio __picked_button = Button_Gpio(PIN_GPIO_BUTTON_PICKED);
        Button_Gpio __packed_button = Button_Gpio(PIN_GPIO_BUTTON_PACKED);

        Adafruit_NeoPixel __leds = Adafruit_NeoPixel(WS2812B_COUNT, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
        void __test_hardware_SetLed(int position_index, int mode, int red, int green, int blue);  
        void __SetLeds();
        int __led_red_index = -1;
        int __led_green_index = -1;
        int __led_blue_index = -1;

};