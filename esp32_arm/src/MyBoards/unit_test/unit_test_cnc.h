#pragma once
#include "unit_test_base.h"
#include "MyBoards/cnc_board_base.h"

class BoardTestCnc:public BoardTestBase{
    public:
        virtual void LinkBoard(CncBoardBase* board){this->__board=board;};
        void Test_AllHomers(int loop_count);
        // void Test_Home_Alpha(int loop_count);
        // void Test_Home_Beta(int loop_count);
        void Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control);

    protected:
        void LinkCncBoard(CncBoardBase* board){this->__board=board;};

    private:
        CncBoardBase* __board;

};