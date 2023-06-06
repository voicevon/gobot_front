#pragma once

#include "board/board.h"
#include "MyLibs/app_base/app_base.h"

class WaterDropper_App: public AppBase{
    public:
        WaterDropper_App();
        void ExecuteCommand(CommandText* gcode_text) override; 
        void LinkBoard(WaterDropper_Board* board){__board = board;};
        
    private:
        int __robot_row_id = 0;
        WaterDropper_Board* __board;

};