#include "board.h"


void Board_SpringMaker::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Spring-Maker.  Keep smiling :)");
   
}

PositionTrigger* Board_SpringMaker::GetPositionTrigger(uint8_t index){
    
}


// ActuatorStepper* Board_SpringMaker::GetActuator(EnumAxis axis_name) {
//     if (axis_name==AXIS_ALPHA){
//         return &this->stepper_alpha;
//     }else{
//         Serial.print("['Error']  Board_SpringMaker::GetStepper()   axis_name= ");
//         Serial.println(axis_name);
//     }
//     return nullptr;
// }

// SinglePositionTrigger* Board_SpringMaker::GetSingleHomer(EnumAxis axis_name) {
//     if (axis_name==AXIS_ALPHA){
//         return &this->homer_alpha;
//     }else{
//         Serial.print("['Error']  Board_SpringMaker::GetHomer()   axis_name= ");
//         Serial.println(axis_name);
//     }
//     return nullptr;
// }

// AxisHomer* Board_SpringMaker::GetPositionTriggers(EnumAxis axis_name){
//     Logger::Error("Board_SpringMaker::GetPositionTriggers() NOT SUPPORTED");
// }

RobotEef_SpringMaker* Board_SpringMaker::GetEef() {
    return &this->eef;
}


void Board_SpringMaker::EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) {
    if (axis_name == AXIS_ALPHA){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_SpringMaker::EnableMotor()  ");
    }
}

