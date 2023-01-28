#pragma once

#include "MyLibs/basic/board_base.h"
#include <Adafruit_NeoPixel.h>
#include "MyLibs/component/button_gpio.h"

#define WS2812B_COUNT 24
#define PIN_NEO_PIXEL  22   // Arduino pin that connects to NeoPixel
#define PIN_GPIO_BUTTON_WITHDRAW 19
#define PIN_GPIO_BUTTON_SHIPOUT 20

class Twh4_ShipOut_Board: public BoardBase{
    public:
        Twh4_ShipOut_Board();
        void Init() override;
        void SetLed_Red(int position_index); 
        void SetLed_Green(int position_index); 
        void SetLed_Blue(int position_index);  
        
        Button_Gpio* GetButton_Withdraw(){return &__withdraw_button;};
        Button_Gpio* GetButton_Shipout(){return &__shipout_button;};

        void TestLed(int test_loop_count,int test_method, int red, int green, int blue);
        void TestButton(int test_loop_count);

    protected:

    private:
        Button_Gpio __withdraw_button = Button_Gpio(PIN_GPIO_BUTTON_WITHDRAW);
        Button_Gpio __shipout_button = Button_Gpio(PIN_GPIO_BUTTON_SHIPOUT);

        Adafruit_NeoPixel __leds = Adafruit_NeoPixel(WS2812B_COUNT, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
        void __test_hardware_SetLed(int position_index, int mode, int red, int green, int blue);  
        void __SetLeds();
        int __led_red_index = -1;
        int __led_green_index = -1;
        int __led_blue_index = -1;

};