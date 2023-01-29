#include "twh2_m42_switch_output.h"


bool Twh2_M42_Runner_Switch_Output::RunnerLoop(){
    switch (_index){
        case 18:
            // __board->EnableVacuumSwitch(_is_on);
            break;
        case 33:
            // __board->EnableVacuumPump(_is_on);
            break;
        default:
            Logger::Warn("Twh2_M42_Runner_Switch_Output::RunnerLoop()");
            Logger::Print("_index", _index);
            break;
    }
    return true;
}
