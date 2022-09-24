#pragma once

// #include "MyBoards/board_test/board_test_cnc.h"
#include "CNC/board/cnc_board_test.h"
// #include "CNC/board_test/cnc_board_test.h"

#include "board_gobot_house.h"
#include "ESP32Step/src/Stepper.h"

class GobotHouse_2206_BoardTest: public CncBoard_Test{
    public:
        GobotHouse_2206_BoardTest(StepControl* stepControl){this->__stepControl=stepControl;};
        void LinkBoard(CncBoardBase* board) override;
        void Test_room_sensors(int loop_count);
        void Test_EefLoadUnload(int loop_count);
        void Test_StepperDriver_OnAlpha(int loop_count);
        void Test_ServoDriver_OnBeta(int loop_count);

    private:
        Board_GobotHouse_2206* __board;
        StepControl* __stepControl;
};