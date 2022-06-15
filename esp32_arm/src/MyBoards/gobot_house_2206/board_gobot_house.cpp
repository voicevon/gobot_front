#include "board_gobot_house.h"


void Board_GobotHouse_2206::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("[Info] Board_GobotHouse_2206::Init()  Hi Xuming, I am your lovely bot,  GobotHouse. ");
    pinMode(PIN_ALPHA_ENABLE_2109, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2109, OUTPUT);
    digitalWrite(PIN_BETA_ENABLE_2109, HIGH);   // To avoid A4988 is enabled.  sometimes there is an A4988 chip on the board.

    this->EnableMotor('A', false);
    this->eef.Init();
    this->mover_StepperServo.LinkStepper_asAlpha(&this->__alpha_stepper);
    this->mover_StepperServo.LinkServo_asBeta(&this->__beta_servo);
    this->cnc_mover = &this->mover_StepperServo;

}

Stepper* Board_GobotHouse_2206::GetJointStepper(char axis_name) {
    if (axis_name=='A'){
        return &this->__alpha_stepper;
    // }else if (axis_name=='B'){
    //     return &this->stepper_beta;
    }else{
        Serial.print("['Error']  Board_GobotHouse_2206::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }
}
ActuatorServo* Board_GobotHouse_2206::GetJointServo(char axis_name){
    if (axis_name=='B'){
        return &this->__beta_servo;
    // }else if (axis_name=='B'){
    //     return &this->stepper_beta;
    }else{
        Serial.print("['Error']  Board_GobotHouse_2206::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }
}


SingleAxisHomer* Board_GobotHouse_2206::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->__alpha_homer;
    // }else if (axis_name=='B'){
    //     return &this->homer_beta;
    }else{
        Serial.print("['Error']  Board_GobotHouse_2206::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

RobotEef_GobotHouse* Board_GobotHouse_2206::GetEef(){
    return &this->eef;
}

void Board_GobotHouse_2206::EnableMotor(char axis_name, bool enable_it) {
    if (axis_name == 'A'){
        digitalWrite(PIN_ALPHA_ENABLE_2109, !enable_it);   // LOW is enable
    // } else if (axis_name == 'B'){
    //     digitalWrite(PIN_BETA_ENABLE_2109, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_GobotHouse_2206::EnableMotor()  wrong parameter");
    }
}

// void Board_GobotHouse_2206::SingleMotorMoveTo(char motor_name, float motor_position) {

// }         
