#pragma once
#include "MyLibs/basic/board_base.h"
#define PIN_YUNXING 27  //D5
#define PIN_TINGZHI 26  //D6
#define PIN_GUZHANG 25  //D7




class GuangDa_ShuiWu_SensorBoard: public BoardBase{
    public:
        void Init() override;
    private:
        void __InitHardware();

};