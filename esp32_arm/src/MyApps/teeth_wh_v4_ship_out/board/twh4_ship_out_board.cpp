#include "twh4_ship_out_board.h"

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3




// Index number
#define PIN_POSITION_TRIGGER_X 17

#define PIN_ALPHA_DIR 14 //32  
#define PIN_ALPHA_STEP 12 //26   
#define PIN_ALPHA_ENABLE 13

// #define PIN_POSITION_TRIGGER_X  128
// #define PIN_POSITION_TRIGGER 1


// // Index number
#define POSITION_TRIGGER_ALPHA 0

Twh4_ShipOut_Board::Twh4_ShipOut_Board(){
    _InitSerialBoard("Hello, I am Twh2Row_Board");
}

void Twh4_ShipOut_Board::Init(){

}
