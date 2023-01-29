#include "twh_m42_switch_output.h"


bool Twh4_Shipout_M42_Runner_Switch_Output::RunnerLoop(){
    // protocol:
    //  _index:   [0:63]: for red,   [64:127]: for green,  [128:192]:  for blue
    int board_led_index = _index % 64;
    switch ( _index / 64){
        case 0:   //red
            __board->SetLed_Red(board_led_index);
            break;
        case 1:   //green
            __board->SetLed_Green(board_led_index);
            break;
        case 2:
            __board->SetLed_Blue(board_led_index);
            break;

        default:
            break;
    }
    return true;
}
