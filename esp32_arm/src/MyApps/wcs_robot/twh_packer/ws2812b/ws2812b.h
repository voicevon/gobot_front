#pragma once

#include <Adafruit_NeoPixel.h>
#include "Mqtt/mqtt_subscriber_manager.h"

#define WS2812B_COUNT 24


class WS2812B: public MqttSubscriberBase{
    public:
        void Link_Adrafruit_NeoPixel(Adafruit_NeoPixel* neo_pixel) ;
        void __SetLeds();
        void SetLed_Red(int position_index); 
        void SetLed_Green(int position_index); 
        void SetLed_Blue(int position_index);  
        void Clear();
        void TestLed(int test_loop_count,int test_method, int red, int green, int blue);

    private:
        void onMessage(const char* payload, uint16_t payload_len) override;
        Adafruit_NeoPixel* __neo_pixel;
        int __led_red_index = -1;
        int __led_green_index = -1;
        int __led_blue_index = -1;
        
        void __test_hardware_SetLed(int position_index, int mode, int red, int green, int blue);  

};