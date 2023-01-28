#pragma once
#include "MyLibs/basic/board_base.h"


#define PIN_YUNXING_4 13
#define PIN_TINGZHI_4 32
#define PIN_GUZHANG_4 4
#define PIN_YUNXING_2 25
#define PIN_TINGZHI_2 27
#define PIN_GUZHANG_2 14


class GuangDa_ShuiWu_GateBoard: public BoardBase{
    public:
        void Init() override;
    private:
        void __InitHardware();

};