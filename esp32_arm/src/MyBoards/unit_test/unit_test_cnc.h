#pragma once
#include "unit_test_base.h"
#include "MyBoards/cnc_board_base.h"

class UnitTestCnc:public UnitTestBase{
    public:
        void LinkBoard(CncBoardBase* board){this->__board=board;};
    private:
        CncBoardBase* __board;

};