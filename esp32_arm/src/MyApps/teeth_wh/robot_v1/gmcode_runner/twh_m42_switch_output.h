#pragma once
#include "Robot/mcode_runner/m42_runner_switch_output.h"
#include "MyApps/teeth_wh/board/teeth_wh_board.h"


class Twh_M42_Runner_Switch_Output: public M42_Runner_Switch_Output{
    public:
        bool RunnerLoop() override;
        void Init(TeethWarehouse_Board* board) {this->__board = board;};
    private:
        TeethWarehouse_Board* __board;
};