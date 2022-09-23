#include "board_gobot_house.h"
#include "CNC/solution/scara/cnc_scara.h"
#include "../cnc_2206/actuator_alpha.h"

void Board_GobotHouse_2206::__Init_Hardware(){
    //Init GPIO
    pinMode(PIN_ALPHA_ENABLE_2109, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2109, OUTPUT);
    digitalWrite(PIN_BETA_ENABLE_2109, HIGH);   // To avoid A4988 is enabled.  sometimes there is an A4988 chip on the board.
    // Init Actuator alpha
    GobotHouse_ActuatorAlphaMechanic alpha_mechanic;
    this->__alpha_stepper.setInverseRotation(true);   //TODO:  inside app_mover.
    this->__actuator_alpha.LinkStepper(&this->__alpha_stepper, alpha_mechanic.GetStepsPerCncUnit());
    this->__alpha_range.InitCncRange(-PI, TWO_PI);
    this->__alpha_range.InitActuatorRange(DEG_TO_RAD * (-350), DEG_TO_RAD * 10);
    this->__actuator_alpha.LinkRangeConstraint(&this->__alpha_range);
    
    // Init actuator beta
	this->__servo_beta.setPeriodHertz(50);
    this->__servo_beta.attach(PIN_CNC_BETA_SERVO);
    this->__actuator_beta.LinkServo(&this->__servo_beta, true);

}

void Board_GobotHouse_2206::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("[Info] Board_GobotHouse_2206::Init()  Hi Xuming, I am your lovely bot,  GobotHouse_2206. ");

    this->__Init_Hardware();
    this->EnableMotor(AXIS_ALPHA, false);

    this->eef.Init();
    this->eef.Run(EEF_CODE_UNLOAD);
    // this->mover_StepperServo.LinkStepControl(this->__stepControl);
    // this->mover_StepperServo.LinkStepper_asAlpha(&this->__actuator_alpha);
    // this->mover_StepperServo.LinkServo_asBeta(&this->__actuator_beta);
    // this->cnc_mover = &this->mover_StepperServo;
}

// ActuatorBase* Board_GobotHouse_2206::GetActuator(EnumAxis axis_name) {
//     if (axis_name==AXIS_ALPHA){
//         return &this->__actuator_alpha;
//     // }else if (axis_name=='B'){
//     //     return &this->stepper_beta;
//     }else{
//         Serial.print("[Error]  Board_GobotHouse_2206::GetActuator()   axis_name= ");
//         Serial.println(axis_name);
//     }
//     return nullptr;
// }

ActuatorServo* Board_GobotHouse_2206::GetActuaorBeta(char axis_name){
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

SinglePositionTrigger* Board_GobotHouse_2206::GetSingleHomer(EnumAxis axis_name) {
    if (axis_name=='A'){
        return &this->__alpha_homer;
    }else if (axis_name=='B'){
        return &this->__beta_homer;
    }else{
        Serial.print("['Error']  Board_GobotHouse_2206::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
}

AxisHomer* Board_GobotHouse_2206::GetPositionTriggers(EnumAxis axis_name){
    Logger::Error("Board_GobotHouse_2206::GetPositionTriggers()  NOT SUPPORTED");
}

RobotEef_GobotHouse* Board_GobotHouse_2206::GetEef(){
    return &this->eef;
}

void Board_GobotHouse_2206::EnableMotor(EnumAxis axis_name, bool enable_it) {
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

