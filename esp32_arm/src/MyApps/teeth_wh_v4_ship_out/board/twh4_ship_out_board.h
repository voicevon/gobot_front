#pragma once

#include "MyLibs/basic/board_base.h"

class Twh4_ShipOut_Board: public BoardBase{
    public:
        Twh4_ShipOut_Board();
        void Init() override;
        void SetLed(int mode);  // 0= OFF, 1= ON, 2= FLASH
    protected:

    private:

        


        

};