#include "board.h"



#define POSITION_TRIGGER_ALPHA 0

Twh_Packer_Board::Twh_Packer_Board(){
    _InitSerialBoard("Hello, I am Twh2Row_Board");
}

void Twh_Packer_Board::Init(){
    __ws2812b.Link_Adrafruit_NeoPixel(&__neo_pixel);
}





