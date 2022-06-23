#pragma once

#include "MyBoards/board_test/board_test_cnc.h"
#include "board_vsc.h"

class Vsc_BoardTest: public BoardTestCnc{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_AngleSensor(int loop_count);
        void Test_Motor(int loop_count);


    private:
        Vsc_Board* __board;

};