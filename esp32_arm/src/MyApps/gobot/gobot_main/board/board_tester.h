#pragma once

// #include "MyBoards/board_test/board_test_cnc.h"
// #include "CNC/board/cnc_board_test.h"
// #include "CNC/board_test/cnc_board_test.h"

#include "board.h"

class GobotMain_BoardTest{
    public:
        void LinkBoard(GobotMain_Board* board);
        void Test_room_sensors(int loop_count);
        void Test_EefUpDown(int loop_count);
        void Test_EefLoadUnload(int loop_count);
        void Test_Eef_Full(int loop_count);

    private:
        GobotMain_Board* __board;

};