#pragma once
#include "Robot/board/board_base.h"


#define PIN_YUNXING_4 13
#define PIN_ZIDONG_4 32
#define PIN_GUZHANG_4 4
#define PIN_YUNXING_2 25
#define PIN_ZIDONG_2 27
#define PIN_GUZHANG_2 14


class GuangDa_ShuiWu_GateBoard: public BoardBase{
    public:
        void Init() override;
    private:
        void __InitHardware();

};




// remote gpio      4-25   4-26         4-27               2-25       2-26              2-27    ----wrong!

// remote gpio      2-25   2-26         2-27                  4-25       4-26              4-27
// remote mean     2#guzhang   2#yungixng  2#shoudong         guzhang     yunxing        shoudong
// remote           2#P3        2#P2          2#P1             4#P3        4#P2           4#P1
// Relay            LP6         LP5           LP4              LP3         LP2            LP1
// line number       6.         5,            4,                 3,         2,             1

// PLC                       2#yunxing                                  4#yinxing


// line number       1.       2,               3               4,         5,               6    -----  Wrong !
//      
// PLC            