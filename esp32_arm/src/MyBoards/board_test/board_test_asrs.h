#pragma once
#include "board_test_base.h"
#include "MyBoards/asrs_agv_2205/sub_boards/board_asrs_2205.h"

class UnitTestAsrs:public BoardTestBase{
    public:
        void LinkBoard(BoardAsrs* board){this->__board=board;};
        void Test_Sharp_IrSensor(int loop_count);

    private:
        BoardAsrs* __board;
};