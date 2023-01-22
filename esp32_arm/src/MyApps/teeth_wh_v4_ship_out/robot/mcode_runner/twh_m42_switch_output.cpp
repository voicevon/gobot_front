#include "twh_m42_switch_output.h"


bool Twh4_Shipout_M42_Runner_Switch_Output::RunnerLoop(){
    __board->SetLed(_index, 1, 0, 100, 0);
    return true;
}
