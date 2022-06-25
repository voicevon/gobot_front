#pragma once

#include "MyBoards/board_test/board_test_cnc.h"
#include "board_all_in_one_2205.h"

class AsarAgv_BoardTest: public BoardTestCnc{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_room_sensors(int loop_count);
        void Test_EefUpDown(int loop_count);
        void Test_EefLoadUnload(int loop_count);
    private:
        CncBoardBase* __board;

};