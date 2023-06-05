#include "board_gobot_house.h"
#include "CNC/arm_solution/scara_xy_ab/scara_xy_ab_arm_solution.h"

void Board_GobotHouse_2206::__Init_Hardware(){
    //Init GPIO
    pinMode(PIN_ALPHA_ENABLE_2109, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2109, OUTPUT);
    digitalWrite(PIN_BETA_ENABLE_2109, HIGH);   // To avoid A4988 is enabled.  sometimes there is an A4988 chip on the board.
    
    // Init actuator beta
	this->__servo_beta.setPeriodHertz(50);
    this->__servo_beta.attach(PIN_CNC_BETA_SERVO);
    this->__actuator_beta.LinkServo(&this->__servo_beta);

}

void Board_GobotHouse_2206::Init(){
    Serial.begin(115200);
    Serial.println("[Info] Board_GobotHouse_2206::Init()  Hi Xuming, I am your lovely bot,  GobotHouse_2206. ");

    this->__Init_Hardware();
    this->EnableMotor(AXIS_ALPHA, false);

    this->eef.Init();
    this->eef.Run(EEF_CODE_UNLOAD);

    __all_position_triggers[0].Init('a',PIN_HOME_ALHPA_2109, LOW);
    __all_position_triggers[1].Init('b',PIN_HOME_BETA_2109, LOW);
    gs_PositionTrigger_Array::Instance().Init(__all_position_triggers,2);
}

CncActuatorServo* Board_GobotHouse_2206::GetActuaorBeta(char axis_name){
    if (axis_name=='B'){
        return &this->__actuator_beta;
    // }else if (axis_name=='B'){
    //     return &this->stepper_beta;
    }else{
        Serial.print("[Error]  Board_GobotHouse_2206::GetActuaorBeta()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
}

void Board_GobotHouse_2206::EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) {
    if (axis_name == AXIS_ALPHA){
        digitalWrite(PIN_ALPHA_ENABLE_2109, !enable_it);   // LOW is enable
    } else if (axis_name == AXIS_BETA){
        // digitalWrite(PIN_BETA_ENABLE_2109, !enable_it);   // LOW is enable
        // Beta joint is using servo.
    } else {
        Serial.print("[Warn] Board_GobotHouse_2206::EnableMotor()  wrong parameter axis_name = ");
        Serial.println(axis_name);
    }
}

