#include "twh4_ship_out_board.h"

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3





#define POSITION_TRIGGER_ALPHA 0
#define BRIGHT_VALUE 11

Twh4_ShipOut_Board::Twh4_ShipOut_Board(){
    _InitSerialBoard("Hello, I am Twh2Row_Board");
}



void Twh4_ShipOut_Board::Init(){
    __leds.begin();
    __leds.show();
    __leds.clear();
}

// 0= OFF, 1= ON, 2= FAST_FLASH, 3= SLOW_FLASH,  4= IN_TURN
void Twh4_ShipOut_Board::SetLed(int position_index, int mode, int red, int green, int blue) {
    __leds.clear();
    if (mode ==1){
        __leds.setPixelColor(2 * position_index, red, green, blue);
        __leds.setPixelColor(2 * position_index+1, red, green, blue);
        __leds.show();
    }
}
void Twh4_ShipOut_Board::ClearLeds(){
    __leds.clear();
}


void Twh4_ShipOut_Board::TestLed(int test_loop_count,int test_method, int red, int green, int blue){

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
                    this->SetLed(position_index, 1, red, green, blue);  // turn off all leds
                    Logger::Print("Position", position_index);
                    delay(1000);
                }
                break;
            case 3:
                for (int position_index=0; position_index <12; position_index++){
                    this->SetLed(position_index, 1, red, green, blue);  // turn off all leds
                    Logger::Print("Position", position_index);
                    delay(1000);
                }
                break;
            default:
                break;
        }
    }
}

void Twh4_ShipOut_Board::TestButton(int test_loop_count){

}

