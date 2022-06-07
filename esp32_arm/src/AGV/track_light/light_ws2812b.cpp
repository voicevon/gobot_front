#include "light_ws2812b.h"


Light_WS2812B::Light_WS2812B(uint16_t ledWs2812B_counts){
    this->__LedWs2812B_counts = ledWs2812B_counts;
    // this->pixels = new Adafruit_NeoPixel(ledWs2812B_counts, pin_WS2812_LED, NEO_GRB + NEO_KHZ800);
    // this->pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    // Serial.println("Light_WS2812B::Light_WS2812B() is OK." );
}

void Light_WS2812B::TurnOn(bool turn_on){
    uint8_t c = 0;
    if (turn_on)
        c = 255;
    for(int i=0; i< this->__LedWs2812B_counts; i++) { // For each pixel...
        this->__pixels->setPixelColor(i, this->__pixels->Color(c,c,c));
        this->__pixels->show();   // Send the updated pixel colors to the hardware.
    }
}