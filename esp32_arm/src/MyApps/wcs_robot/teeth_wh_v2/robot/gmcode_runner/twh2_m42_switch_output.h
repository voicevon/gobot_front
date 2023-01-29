#pragma once
#include "Robot/mcode_runner/m42_runner_switch_output.h"
#include "MyApps/wcs_robot/teeth_wh_v2/board/teeth_wh_board.h"


class Twh2_M42_Runner_Switch_Output: public M42_Runner_Switch_Output{
    public:
        bool RunnerLoop() override;
        void Init(Twh2_Board* board) {this->__board = board;};
    private:
        Twh2_Board* __board;
};