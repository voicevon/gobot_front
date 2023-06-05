#include "board.h"



#define POSITION_TRIGGER_ALPHA 0

Twh_Packer_Board::Twh_Packer_Board(){
    _InitSerial("Hello, I am Twh2Row_Board");
}

void Twh_Packer_Board::Init(){
    __ws2812b.Link_Adrafruit_NeoPixel(&__neo_pixel);
}

void Twh_Packer_Board::BootTest(){
    #define DEALY_LED 300
    for(int i=0; i<12; i++){
        __ws2812b.SetLed_Red(i);
        __ws2812b.SetLed_Green(13);
        __ws2812b.SetLed_Blue(13);
        delay(DEALY_LED);

        __ws2812b.SetLed_Green(i);
        __ws2812b.SetLed_Red(13);
        __ws2812b.SetLed_Blue(13);
        delay(DEALY_LED);
        
        __ws2812b.SetLed_Blue(i);
        __ws2812b.SetLed_Green(13);
        __ws2812b.SetLed_Red(13);
        delay(DEALY_LED);
    }
    __ws2812b.SetLed_Blue(13);
}




