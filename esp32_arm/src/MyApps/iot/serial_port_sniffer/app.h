#pragma once

#include "board/board.h"
#include "MyLibs/app_base/app_base.h"

class SerialPortSniffer_App: public AppBase{
    public:
        // SerialPortSniffer_App():AppBase(){};
        // ~SerialPortSniffer_App(){};
        void LinkBoard(SerialPortSniffer_Board* board){__board = board;};
        
    private:
        int __robot_row_id = 0;
        SerialPortSniffer_Board* __board;

};