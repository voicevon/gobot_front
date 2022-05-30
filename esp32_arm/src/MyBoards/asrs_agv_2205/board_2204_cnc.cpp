#include "board_2204_cnc.h"

void Board2204Cnc::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("[Info]  Board2204Cnc::Init() Hi world! ");
    }
    else{
        Serial.println("[Warn] Board2204Cnc::Init().  You should call Init(Adafruit_MCP23X17* )");
    }
}

void Board2204Cnc::Init(Adafruit_MCP23X17* mcp_23018){
    this->__mcp23018 = mcp_23018;
    this->stepper_alpha.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_ALPHA_DIR_2205);
    this->stepper_beta.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_BETA_DIR_2205);
    this->homer_A.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_HOME_Y_2205);
    this->homer_z.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_HOME_Z_2205);
}

Stepper* Board2204Cnc::GetStepper(char axis_name){
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else if (axis_name=='B'){
        return &this->stepper_beta;
    }else{
        Serial.print("['Error']  Board2204Cnc::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }
}

SingleAxisHomer* Board2204Cnc::GetHomer(char axis_name){
    if (axis_name=='Z'){
        return &this->homer_z;
    }else if (axis_name=='A'){
        return &this->homer_A;
    }else{
        Serial.print("['Error']  Board2204Cnc::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

RobotEef_GarmentAsar* Board2204Cnc::GetEef(){
    return &this->eef;
}

void Board2204Cnc::EnableMotor(char axis_name, bool enable_it){
    if (axis_name == 'A'){
        this->__mcp23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE_2205, !enable_it);   // LOW is enable
    } else if (axis_name == 'B'){
        this->__mcp23018->digitalWrite(MC23018_PIN_BETA_ENABLE_2205, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board2204Cnc::EnableMotor()  ");
    }
}

