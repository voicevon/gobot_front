#include "board_gobot_main.h"


void Board_GobotMain::Init(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am Gobot-Chessboard. Good luck......");
 
}

Stepper* Board_GobotMain::GetStepper(char axis_name) {
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else if (axis_name=='B'){
        return &this->stepper_beta;
    }else{
        Serial.print("['Error']  Board_GobotMain::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }

}

SingleAxisHomer* Board_GobotMain::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->homer_alpha;
    }else if (axis_name=='B'){
        return &this->homer_beta;
    }else{
        Serial.print("['Error']  Board_GobotMain::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

RobotEef_GobotMain* Board_GobotMain::GetEef() {
    return &this->eef;
}


void Board_GobotMain::EnableMotor(char axis_name, bool enable_it) {
    if (axis_name == 'A'){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else if (axis_name == 'B'){
        digitalWrite(PIN_BETA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] CncDriver_CoreYZ_2205::EnableMotor()  ");
    }
}

