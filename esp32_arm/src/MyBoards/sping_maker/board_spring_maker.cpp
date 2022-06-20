#include "board_spring_maker.h"


void Board_SpringMaker::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Spring-Maker.  Keep smiling :)");
   
}

ActuatorStepper* Board_SpringMaker::GetJointStepper(char axis_name) {
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else{
        Serial.print("['Error']  Board_SpringMaker::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
}

SingleAxisHomer* Board_SpringMaker::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->homer_alpha;
    }else{
        Serial.print("['Error']  Board_SpringMaker::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
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

