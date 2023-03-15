#include "twh_loop_porter_m42_switch_output.h"


bool Twh_LoopPorter_M42_Runner_Switch_Output::RunnerLoop(){
    // Logger::Debug("Twh_LoopPorter_M42_Runner_Switch_Output::RunnerLoop()");
    // Logger::Print("_index", _index);
    __board->TurnOn_ThisLed_Only(_index);
    return true;
}
