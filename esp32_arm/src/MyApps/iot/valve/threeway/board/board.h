#pragma once

#include "MyLibs/board/board_base.h"



#include "Robot/valve/three_way_valve/three_way_valve.h"

class ThreeWayValveBoard: public BoardBase{
    public:


        ThreeWayValveBoard(){};

        void Init() override;
        ThreeWayValve* GetValve(){return &__valve;};


    private:
        ThreeWayValve __valve;
        BinaryOutput_GPIO __led_go_straight_blue;

};