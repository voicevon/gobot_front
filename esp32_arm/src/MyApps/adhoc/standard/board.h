#pragma once
#include "MyLibs/basic/board_base.h"
#define PIN_STATE_LED 2


class GuangDa_ShuiWu_RouterBoard: public BoardBase{
    public:
        void Init() override;

        
    private:
        void __InitHardware();

};