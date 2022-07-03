#pragma once

#include "MyBoards/board_test/board_test_cnc.h"
#include "board.h"

class GobotMain_BoardTest: public BoardTestCnc{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_room_sensors(int loop_count);
        void Test_EefUpDown(int loop_count);
        void Test_EefLoadUnload(int loop_count);
        void Test_Eef_Full(int loop_count);

    private:
        GobotMain_Board* __board;

};