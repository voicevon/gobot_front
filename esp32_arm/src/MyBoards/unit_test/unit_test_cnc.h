#pragma once
#include "unit_test_base.h"
#include "MyBoards/cnc_board_base.h"

class UnitTestCnc:public UnitTestBase{
    public:
        virtual void LinkBoard(CncBoardBase* board){this->__board=board;};
        void Test_AllHomers(int loop_count);
        void Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control);
        
    private:
        CncBoardBase* __board;

};