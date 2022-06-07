#pragma once
#include "unit_test_base.h"
#include "MyBoards/agv_board_base.h"

class UnitTestAgv:public UnitTestBase{
    public:
        void LinkBoard(AgvBoardbase* board){this->__board=board;};
        void Test_TrackSensor(int loop_count);
        void Test_ObstacleSensor(int loop_count);

    private:
        AgvBoardbase* __board;

};