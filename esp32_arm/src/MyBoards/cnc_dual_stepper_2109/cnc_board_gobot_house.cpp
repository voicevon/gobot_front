#include "cnc_board_gobot_house.h"


void CncBoard_AB_AlphaBeta::Init(){
    
}

Stepper* CncBoard_AB_AlphaBeta::GetStepper(char axis_name) {
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else if (axis_name=='B'){
        return &this->stepper_beta;
    }else{
        Serial.print("['Error']  CncBoard_AB_AlphaBeta::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }

}

SingleAxisHomer* CncBoard_AB_AlphaBeta::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->homer_alpha;
    }else if (axis_name=='B'){
        return &this->homer_beta;
    }else{
        Serial.print("['Error']  CncBoard_AB_AlphaBeta::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

void CncBoard_AB_AlphaBeta::EnableMotor(char axis_name, bool enable_it) {
    if (axis_name == 'A'){
        digitalWrite(PIN_ALPHA_ENABLE_2109, !enable_it);   // LOW is enable
    } else if (axis_name == 'B'){
        digitalWrite(PIN_BETA_ENABLE_2109, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] CncDriver_CoreYZ_2205::EnableMotor()  ");
    }
}

