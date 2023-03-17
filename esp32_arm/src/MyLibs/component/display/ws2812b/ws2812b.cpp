#include "ws2812b.h"
#define BRIGHT_VALUE 11  // TODO:  rename to BRIGHTNESS


void WS2812B::Init(){
    __leds.begin();
    __leds.show();
    __leds.clear();
}

void WS2812B::onMessage(const char* payload, uint16_t payload_len){
    
}


// void WS2812B::Init_Remotable(const char* mqtt_topic){
//     _mqtt_topic =  mqtt_topic;
//     _mqtt
// }

void WS2812B::__SetLeds(){
    int red, green, blue;
    for (int i=0; i<12; i++){
        red = 0; green = 0; blue = 0;
        if (__led_red_index == i) red = BRIGHT_VALUE;
        if (__led_green_index == i) green = BRIGHT_VALUE;
        if (__led_blue_index == i) blue = BRIGHT_VALUE;
        __leds.setPixelColor(2 * i, red, green, blue);
        __leds.setPixelColor(2 * i+1, red, green, blue);
    }
    __leds.show();
}


void WS2812B::SetLed_Red(int position_index){
    __led_red_index = position_index;
    __SetLeds();
}

void WS2812B::SetLed_Green(int position_index){
    __led_green_index = position_index;
    __SetLeds();
}

void WS2812B::SetLed_Blue(int position_index){
    __led_blue_index = position_index;
    __SetLeds();
}

void WS2812B::TestLed(int test_loop_count,int test_method, int red, int green, int blue){

    for (int loop_index=0; loop_index < test_loop_count; loop_index++){
        switch (test_method){    
            case 1:
                for (int i=0; i<WS2812B_COUNT; i++){
                    Logger::Print("index",i);
                    __leds.clear();
                    __leds.setPixelColor(i, 1, red, green, blue);
                    __leds.show();
                    delay(1000);
                    __leds.clear();
                    __leds.setPixelColor(i, 1, red, green, blue);
                    __leds.show();
                    delay(1000);
                    __leds.clear();
                    __leds.setPixelColor(i, 1, red, green, blue);
                    __leds.show();
                    delay(1000);
                }
                break;
            case 2:
                for (int position_index=0; position_index <12; position_index++){
                    this->__test_hardware_SetLed(position_index, 1, red, green, blue);  // turn off all leds
                    Logger::Print("Position", position_index);
                    delay(1000);
                }
                break;
            case 3:
                for (int position_index=0; position_index <12; position_index++){
                    this->__test_hardware_SetLed(position_index, 1, red, green, blue);  // turn off all leds
                    Logger::Print("Position", position_index);
                    delay(1000);
                }
                break;
            default:
                break;
        }
    }
}

// 0= OFF, 1= ON, 2= FAST_FLASH, 3= SLOW_FLASH,  4= IN_TURN
void WS2812B::__test_hardware_SetLed(int position_index, int mode, int red, int green, int blue) {
    // __leds.clear();
    if (mode ==1){   // TODO: remove arg:  mode
        __leds.setPixelColor(2 * position_index, red, green, blue);
        __leds.setPixelColor(2 * position_index+1, red, green, blue);
    }
}