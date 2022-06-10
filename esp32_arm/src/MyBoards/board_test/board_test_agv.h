#pragma once
#include "board_test_base.h"
#include "MyBoards/agv_board_base.h"

class UnitTestAgv:public BoardTestBase{
    public:
        void LinkBoard(AgvBoardbase* board){this->__board=board;};
        void Test_TrackSensor(int loop_count);
        void Test_ObstacleSensor(int loop_count);
        void Test_TrackLight(int loop_count);
        void Test_RfidReader(int loop_count);
        void Test_Mover(int loop_count);
        void Test_Battery(int loop_count);
        
    private:
        AgvBoardbase* __board;

};