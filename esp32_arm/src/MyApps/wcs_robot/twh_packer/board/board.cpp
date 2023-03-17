#include "board.h"

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3
//  x-end   GPIO 17
//  y-end   GPIO 4





#define POSITION_TRIGGER_ALPHA 0

Twh_Packer_Board::Twh_Packer_Board(){
    _InitSerialBoard("Hello, I am Twh2Row_Board");
}



void Twh_Packer_Board::Init(){
    
    // __leds.begin();
    // __leds.show();
    // __leds.clear();
}





// void Twh_Packer_Board::ClearLeds(){
//     __leds.clear();
// }




void Twh_Packer_Board::TestButton(int test_loop_count){

}

