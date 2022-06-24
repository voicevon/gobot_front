#pragma once

#include "MyBoards/board_test/board_test_cnc.h"
#include "board_vsc.h"

class Vsc_BoardTest: public BoardTestCnc{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_AngleSensor(int loop_count);
        void Test_Motor(int level);
        void Test_Offset(int loop_count);
        void Test_AllHomers(int loop_count) override;


    private:
        Vsc_Board* __board;
        void __TestOffset(float value);
};