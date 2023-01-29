#pragma once
#include "Robot/mcode_runner/m42_runner_switch_output.h"
#include "MyApps/wcs_robot/teeth_wh_v2_row/board/twh2_row_board.h"


class Twh2Row_M42_Runner_Switch_Output: public M42_Runner_Switch_Output{
    public:
        bool RunnerLoop() override;
        void Init(Twh2Row_Board* board) {this->__board = board;};
    private:
        Twh2Row_Board* __board;
};