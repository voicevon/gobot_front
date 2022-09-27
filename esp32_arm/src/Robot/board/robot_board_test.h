#pragma once
#include "MyBoards/board_test/board_test_base.h"
#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "robot_board_base.h"

class RobotBoard_Test:public BoardTestBase{
    public:
        virtual void LinkBoard(RobotBoardBase* board){this->__board=board;};
        void Test_SinglePositionTrigger(PositionTrigger* homer, int loop_count);
        void Test_AxisHomer(AxisHomer* position_triggers, int loop_count);

    protected:
        void LinkCncBoard(RobotBoardBase* board){this->__board=board;};


    private:
        RobotBoardBase* __board;

};