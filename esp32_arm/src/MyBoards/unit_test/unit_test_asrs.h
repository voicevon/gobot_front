#pragma once
#include "unit_test_base.h"
#include "MyBoards/asrs_agv_2205/board_asrs_2205.h"

class UnitTestAsrs:public UnitTestBase{
    public:
        void LinkBoard(BoardAsrs* board){this->__board=board;};
        void Test_Sharp_IrSensor(int loop_count);

    private:
        BoardAsrs* __board;
};