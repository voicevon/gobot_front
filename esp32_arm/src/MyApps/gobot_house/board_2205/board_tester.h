#pragma once

#include "CNC/board/cnc_board_test.h"
// #include "CNC/board_test/cnc_board_test.h"
#include "board_gobot_house.h"

class GobotHouse_BoardTest: public CncBoard_Test{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_room_sensors(int loop_count);
        // void Test_EefLoadUnload(int loop_count);
        
    private:
        Board_GobotHouse* __board;

};