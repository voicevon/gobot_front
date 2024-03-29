#pragma once
#include "Robot/mcode_runner/m42_runner_switch_output.h"
#include "../../board/board.h"


class Silicon_Pump_M42_Runner_Switch_Output: public M42_Runner_Switch_Output{
    public:
        bool RunnerLoop() override;
        void Init(Silicon_Pump_Board* board) {__board = board;};
    private:
        Silicon_Pump_Board* __board;
};