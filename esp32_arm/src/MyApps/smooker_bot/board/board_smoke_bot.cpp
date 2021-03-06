#include "board_smoke_bot.h"


void Board_SmokeMachine::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Spring-Maker.  Keep smiling :)");
   
}

void Board_SmokeMachine::SayHello() {
    
}


// Stepper* Board_SmokeMachine::GetStepper(char axis_name) {
//     if (axis_name=='A'){
//         return &this->stepper_alpha;
//     }else{
//         Serial.print("['Error']  Board_SpringMaker::GetStepper()   axis_name= ");
//         Serial.println(axis_name);
//     }

// }

SingleAxisHomer* Board_SmokeMachine::GetHomer(EnumAxis axis_name) {
    if (axis_name== AXIS_ALPHA){
        return &this->homer_alpha;
    }else{
        Serial.print("['Error']  Board_SpringMaker::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
}

RobotEef_SmokeMachine* Board_SmokeMachine::GetEef() {
    return &this->eef;
}


void Board_SmokeMachine::EnableMotor(EnumAxis axis_name, bool enable_it) {
    if (axis_name == AXIS_ALPHA){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_SpringMaker::EnableMotor()  ");
    }
}

