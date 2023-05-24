#include "ws2812b.h"
#define BRIGHT_VALUE 88  // TODO:  rename to BRIGHTNESS


void WS2812B::Link_Adrafruit_NeoPixel(Adafruit_NeoPixel* neo_pixel){
        // Adafruit_NeoPixel __leds = Adafruit_NeoPixel(WS2812B_COUNT, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
    __neo_pixel = neo_pixel;
    __neo_pixel->begin();
    __neo_pixel->show();
    __neo_pixel->clear();
}

void WS2812B::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    // Logger::Debug("WS2812B::onMessage()");
    // Logger::Print("payload", payload);
    int led_index = atoi(payload);
    // int color_index = led_index / 12;
    __led_red_index = 99;
    __led_green_index = led_index % 100;
    __led_blue_index = led_index / 100;

    __SetLeds();
}



void WS2812B::__SetLeds(){
    int red, green, blue;
    for (int i=0; i<12; i++){
        red = 0; green = 0; blue = 0;
        if (__led_red_index == i) red = BRIGHT_VALUE;
        if (__led_green_index == i) green = BRIGHT_VALUE;
        if (__led_blue_index == i) blue = BRIGHT_VALUE;
        __neo_pixel->setPixelColor(2 * i, red, green, blue);
        __neo_pixel->setPixelColor(2 * i+1, red, green, blue);
    }
    __neo_pixel->show();
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


void WS2812B::Clear(){
    __neo_pixel->clear();
    __neo_pixel->show();
}

void WS2812B::TestLed(int test_loop_count,int test_method, int red, int green, int blue){

    for (int loop_index=0; loop_index < test_loop_count; loop_index++){
        switch (test_method){    
            case 1:
                for (int i=0; i<WS2812B_COUNT; i++){
                    Logger::Print("index",i);
                    __neo_pixel->clear();
                    __neo_pixel->setPixelColor(i, red, green, blue);
                    __neo_pixel->show();
                    delay(1000);
                    __neo_pixel->clear();
                    __neo_pixel->setPixelColor(i, red, green, blue);
                    __neo_pixel->show();
                    delay(1000);
                    __neo_pixel->clear();
                    __neo_pixel->setPixelColor(i, red, green, blue);
                    __neo_pixel->show();
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
        __neo_pixel->setPixelColor(2 * position_index, red, green, blue);
        __neo_pixel->setPixelColor(2 * position_index+1, red, green, blue);
    }
}