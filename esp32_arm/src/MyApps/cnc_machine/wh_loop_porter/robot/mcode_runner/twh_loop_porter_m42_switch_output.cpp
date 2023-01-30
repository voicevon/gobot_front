#include "twh_loop_porter_m42_switch_output.h"


bool Twh2Row_M42_Runner_Switch_Output::RunnerLoop(){
    __board->TurnOn_ThisLed_Only(_index);
    return true;
}
