#include "board_gobot_house.h"


void Board_GobotHouse::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("[Info] Board_GobotHouse::Init()  Hi Xuming, I am your lovely bot,  GobotHouse. ");
}

Stepper* Board_GobotHouse::GetStepper(char axis_name) {
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else if (axis_name=='B'){
        return &this->stepper_beta;
    }else{
        Serial.print("['Error']  Board_GobotHouse::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }
}

SingleAxisHomer* Board_GobotHouse::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->homer_alpha;
    }else if (axis_name=='B'){
        return &this->homer_beta;
    }else{
        Serial.print("['Error']  Board_GobotHouse::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

RobotEef_GobotHouse* Board_GobotHouse::GetEef(){
    return &this->eef;
}

void Board_GobotHouse::EnableMotor(char axis_name, bool enable_it) {
    if (axis_name == 'A'){
        digitalWrite(PIN_ALPHA_ENABLE_2109, !enable_it);   // LOW is enable
    } else if (axis_name == 'B'){
        digitalWrite(PIN_BETA_ENABLE_2109, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_GobotHouse::EnableMotor()  wrong parameter");
    }
}

