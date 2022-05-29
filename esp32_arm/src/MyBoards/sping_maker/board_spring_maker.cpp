#include "board_spring_maker.h"


void Board_SpringMaker::Init(){
    
}

Stepper* Board_SpringMaker::GetStepper(char axis_name) {
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else{
        Serial.print("['Error']  Board_SpringMaker::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }

}

SingleAxisHomer* Board_SpringMaker::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->homer_alpha;
    }else{
        Serial.print("['Error']  Board_SpringMaker::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

RobotEef_SpringMaker* Board_SpringMaker::GetEef() {
    return &this->eef;
}


void Board_SpringMaker::EnableMotor(char axis_name, bool enable_it) {
    if (axis_name == 'A'){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_SpringMaker::EnableMotor()  ");
    }
}
