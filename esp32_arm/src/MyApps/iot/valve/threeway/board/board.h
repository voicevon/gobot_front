#pragma once

#include "MyLibs/board/board_base.h"
#include "MyLibs/component/binary_output_gpio.h"


class ThreeWayValveBoard: public BoardBase{
    public:
        void Init() override;

    private:
        BinaryOutput_GPIO __led_go_straight_blue;

};