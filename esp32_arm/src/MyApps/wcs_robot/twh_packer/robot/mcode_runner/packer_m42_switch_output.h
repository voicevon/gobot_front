#pragma once
#include "Robot/mcode_runner/m42_runner_switch_output.h"
// #include "MyApps/wcs_robot/teeth_wh_v4_ship_out/board/twh4_ship_out_board.h"
#include "../../board/board.h"


class Twh_Packer_M42_Runner_Switch_Output: public M42_Runner_Switch_Output{
    public:
        bool RunnerLoop() override;
        void Init(Twh_Packer_Board* board) {this->__board = board;};
    private:
        Twh_Packer_Board* __board;
};