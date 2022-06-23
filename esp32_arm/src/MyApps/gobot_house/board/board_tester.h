#pragma once

#include "MyBoards/board_test/board_test_cnc.h"
#include "board_gobot_house.h"

class GobotHouse_BoardTest: public BoardTestCnc{
    public:
        void LinkBoard(CncBoardBase* board) override;
        void Test_room_sensors(int loop_count);
        void Test_EefLoadUnload(int loop_count);
        
    private:
        Board_GobotHouse* __board;

};