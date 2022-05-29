#include "cnc_board_gobot_house.h"

// void BoardPart_Cnc::Init(Adafruit_MCP23X17* mcp_23018){
//     this->__mcp23018 = mcp_23018;
//     this->stepper_alpha.Init_mcp23018(mcp_23018, MC23018_PIN_ALPHA_DIR);
//     this->stepper_beta.Init_mcp23018(mcp_23018, MC23018_PIN_BETA_DIR);
//     this->homer_y.Init_mcp23018(mcp_23018, MC23018_PIN_HOME_Y);
//     this->homer_z.Init_mcp23018(mcp_23018, MC23018_PIN_HOME_Z);
// }

void BoardPart_Cnc::Init(){
    
}

Stepper* BoardPart_Cnc::GetStepper(char axis_name) {

}

SingleAxisHomer* BoardPart_Cnc::GetHomer(char axis_name) {

}

void BoardPart_Cnc::EnableMotor(char axis_name, bool enable_it) {

}

// void BoardPart_Cnc::EnableMotor_alpha(bool enable_it){
//     // this->__mcp23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE, !enable_it);   // LOW is enable
// }

// void BoardPart_Cnc::EnableMotor_beta(bool enable_it){
//     // this->__mcp23018->digitalWrite(MC23018_PIN_BETA_ENABLE, !enable_it);   // LOW is enable
// }
