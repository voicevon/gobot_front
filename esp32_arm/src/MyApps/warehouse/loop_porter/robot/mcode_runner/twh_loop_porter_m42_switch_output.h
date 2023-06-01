#pragma once
#include "Robot/mcode_runner/m42_runner_switch_output.h"
#include "../../board/board.h"


class Twh_LoopPorter_M42_Runner_Switch_Output: public M42_Runner_Switch_Output{
    public:
        bool RunnerLoop() override;
        void Init(Twh_LoopPorter_Board* board) {__board = board;};
    private:
        Twh_LoopPorter_Board* __board;
};