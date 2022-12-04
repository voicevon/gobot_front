#include "twh_m42_switch_output.h"


bool Twh_M42_Runner_Switch_Output::RunnerLoop(){
    if (_index == 0){
        __board->EnableVacuumPump(_is_on);
        return true;

    }
}
