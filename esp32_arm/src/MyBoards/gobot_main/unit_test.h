#pragma once

#include "MyBoards/unit_test/unit_test_cnc.h"
#include "MyBoards/gobot_main/board_gobot_main.h"

class GobotMain_BoardTest: public BoardTestCnc{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_room_sensors(int loop_count);

    private:
        Board_GobotMain* __board;


};