#include "board.h"


void Board_SpringMaker::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Spring-Maker.  Keep smiling :)");
    
    PositionTrigger_Array::Instance().Init(_all_position_triggers, POSITION_TRIGGERS_COUNT);
    _all_position_triggers[0].Init('U',PIN_HOME_ALPHA_2201, LOW);
}



void Board_SpringMaker::EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) {
    if (axis_name == AXIS_ALPHA){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_SpringMaker::EnableMotor()  ");
    }
}

