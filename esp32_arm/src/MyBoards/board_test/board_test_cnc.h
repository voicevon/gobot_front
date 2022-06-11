#pragma once
#include "board_test_base.h"
#include "MyBoards/cnc_board_base.h"

class BoardTestCnc:public BoardTestBase{
    public:
        // virtual void LinkBoard(CncBoardBase* board){this->__board=board;};
        virtual void LinkBoard(CncBoardBase* board);
        virtual void Test_AllHomers(int loop_count){};
        void Test_SingleHomer(int loop_count, char axis_name);
        void Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control);
        void Test_StepperEnablePin(int loop_count, char axis_name);

    protected:
        void LinkCncBoard(CncBoardBase* board){this->__board=board;};


    private:
        CncBoardBase* __board;

};