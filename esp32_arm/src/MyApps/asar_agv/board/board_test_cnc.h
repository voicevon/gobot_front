#pragma once

// #include "CNC/board_test/cnc_board_test.h"
#include "CNC/board/cnc_board_test.h"

#include "board_all_in_one_2205.h"

class AsarAgv_BoardTest: public CncBoard_Test{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_room_sensors(int loop_count);
        void Test_EefUpDown(int loop_count);
        void Test_EefLoadUnload(int loop_count);
    private:
        CncBoardBase* __board;

};