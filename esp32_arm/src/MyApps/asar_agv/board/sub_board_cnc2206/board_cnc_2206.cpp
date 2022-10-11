#include "board_cnc_2206.h"

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
    this->__all_position_triggers[0].Init(mcp_23018, MC23018_PIN_HOME_Y_2205,LOW);  // for axisA
    this->__all_position_triggers[1].Init(mcp_23018, MC23018_PIN_HOME_Z_2205,LOW); // for axisZ
    HomeTrigger_Array::Instance().Init(__all_position_triggers,2);
}

// Stepper* Board2204Cnc::GetStepper(char axis_name){
//     if (axis_name=='A'){
//         return &this->stepper_alpha;
//     }else if (axis_name=='B'){
//         return &this->stepper_beta;
//     }else{
//         Serial.print("['Error']  Board2204Cnc::GetStepper()   axis_name= ");
//         Serial.println(axis_name);
//     }
// }

// PositionTrigger* Board2204Cnc::GetSingleHomer(EnumAxis axis_name){
//     if (axis_name==AXIS_Z){
//         return &this->homer_z;
//     }else if (axis_name==AXIS_ALPHA){
//         return &this->homer_A;
//     }else{
//         Serial.print("['Error']  Board2204Cnc::GetHomer()   axis_name= ");
//         Serial.println(axis_name);
//     }
//     return nullptr;
// }

// RobotEef_GarmentAsar* Board2204Cnc::GetEef(){
//     return &this->eef;
// }

CncActuatorBase* Board2204Cnc::GetActuator(EnumAxis_Inverseinematic axis){
    return nullptr;
}

void Board2204Cnc::EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it){
    if (axis_name == AXIS_ALPHA){
        this->__mcp23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE_2205, !enable_it);   // LOW is enable
    } else if (axis_name == AXIS_BETA){
        this->__mcp23018->digitalWrite(MC23018_PIN_BETA_ENABLE_2205, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board2204Cnc::EnableMotor()  ");
    }
}

