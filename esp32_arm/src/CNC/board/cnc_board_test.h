#pragma once
#include "MyBoards/board_test/board_test_base.h"
// #include "MyBoards/cnc_board_base.h"
#include "CNC/board/cnc_board_base.h"
// #include "CNC/Sensor/position_trigger/position_triggers.h"
#include "CNC/solution/axis_homer/axis_homer.h"


class CncBoard_Test:public BoardTestBase{
    public:
        virtual void LinkBoard(CncBoardBase* board){this->__board=board;};
        void Test_SinglePositionTrigger(SinglePositionTrigger* homer, int loop_count);
        void Test_AxisHomer(AxisHomer* position_triggers, int loop_count);

    protected:
        void LinkCncBoard(CncBoardBase* board){this->__board=board;};


    private:
        CncBoardBase* __board;

};