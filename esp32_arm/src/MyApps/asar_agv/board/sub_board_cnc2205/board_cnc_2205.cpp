#include "board_cnc_2205.h"

void Board2205Cnc::Init(bool is_on_reset){
    Serial.println("[Error] Board2205Cnc::Init().  You should call Init(Adafruit_MCP23X17* )");
}

void Board2205Cnc::Init(Adafruit_MCP23X17* mcp_23018){
    this->__mcp23018 = mcp_23018;
    this->__mcp23018->pinMode(MC23018_PIN_ALPHA_ENABLE_2205, OUTPUT);
    this->__mcp23018->pinMode(MC23018_PIN_BETA_ENABLE_2205, OUTPUT);

    this->stepper_driver_alpha.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_ALPHA_DIR_2205);
    this->stepper_driver_beta.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_BETA_DIR_2205);

    this->homer_y.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_HOME_Y_2205);
    this->homer_z.Init_mcp23018_gpio(mcp_23018, MC23018_PIN_HOME_Z_2205);
}

// ActuatorBase* Board2205Cnc::GetActuator(EnumAxis axis_name){
//     if (axis_name=='A'){
//         return &this->alpha_stepper;
//     }else if (axis_name=='B'){
//         return &this->beta_stepper;
//     }else{
//         Serial.print("['Error']  Board2205Cnc::GetStepper()   axis_name= ");
//         Serial.println(axis_name);
//     }
// }

SinglePositionTriger* Board2205Cnc::GetSingleHomer(EnumAxis axis_name){
    if (axis_name=='Z'){
        return &this->homer_z;
    }else if (axis_name=='Y'){
        return &this->homer_y;
    }else{
        Serial.print("['Error']  Board2205Cnc::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
}

PositionTriggers* Board2205Cnc::GetPositionTriggers(EnumAxis axis_name){
    Logger::Error("Board2205Cnc::GetPositionTriggers() , NOT SUPPORT");
}


RobotEef_GarmentAsar* Board2205Cnc::GetEef(){
    return &this->eef;
}

void Board2205Cnc::EnableMotor(EnumAxis axis_name, bool enable_it){
    if (axis_name == 'A'){
        this->__mcp23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE_2205, !enable_it);   // LOW is enable
    } else if (axis_name == 'B'){
        this->__mcp23018->digitalWrite(MC23018_PIN_BETA_ENABLE_2205, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board2205Cnc::EnableMotor()  ");
    }
}

