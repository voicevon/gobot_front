#include "board_cnc.h"

void BoardPart_Cnc::EnableMotor_alpha(bool enable_it){
    this->__mcp23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE, !enable_it);   // LOW is enable
}

void BoardPart_Cnc::EnableMotor_beta(bool enable_it){
    this->__mcp23018->digitalWrite(MC23018_PIN_BETA_ENABLE, !enable_it);   // LOW is enable
}