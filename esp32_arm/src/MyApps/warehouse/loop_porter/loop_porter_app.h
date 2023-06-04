#pragma once

#include "MyLibs/app_base/cnc_base/cnc_app_base.h"
#include "board/board.h"

class Twh_LoopPorter_App: public CncAppBase{
    public:
        Twh_LoopPorter_App(int row_id);
        void ExecuteCommand(GcodeText* gcode_text) override; 
        void LinkBoard(Twh_LoopPorter_Board* board){__board = board;};
        
    private:
        int __robot_row_id = 0;
        Twh_LoopPorter_Board* __board;

};