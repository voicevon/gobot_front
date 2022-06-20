#include "board_gobot_house.h"
#include "mechanic/alpha_stepper.h"

void Board_GobotHouse_2206::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("[Info] Board_GobotHouse_2206::Init()  Hi Xuming, I am your lovely bot,  GobotHouse. ");
    pinMode(PIN_ALPHA_ENABLE_2109, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2109, OUTPUT);
    digitalWrite(PIN_BETA_ENABLE_2109, HIGH);   // To avoid A4988 is enabled.  sometimes there is an A4988 chip on the board.
    this->EnableMotor('A', false);
    this->eef.Init();
    this->eef.Run(EEF_CODE_UNLOAD);

	this->__beta_servo_driver.setPeriodHertz(50);
    this->__beta_servo_driver.attach(PIN_CNC_BETA_SERVO);
    this->__beta_servo.LinkServo(&this->__beta_servo_driver, true);
    
    GobotHouse2206_AlphaStepper_Mechanic alpha_mechanic;
    this->__stepper_alpha.setInverseRotation(true);
    this->__actuator_alpha.LinkStepper(&this->__stepper_alpha, &alpha_mechanic);

    this->mover_StepperServo.LinkStepper_asAlpha(&this->__actuator_alpha);
    this->mover_StepperServo.LinkServo_asBeta(&this->__beta_servo);
    this->cnc_mover = &this->mover_StepperServo;

}

ActuatorStepper* Board_GobotHouse_2206::GetActuator(char axis_name) {
    if (axis_name=='A'){
        return &this->__actuator_alpha;
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
    return nullptr;
}


SingleAxisHomer* Board_GobotHouse_2206::GetHomer(char axis_name) {
    if (axis_name=='A'){
        return &this->__alpha_homer;
    }else if (axis_name=='B'){
        return &this->__beta_homer;
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
    } else if (axis_name == 'B'){
        // digitalWrite(PIN_BETA_ENABLE_2109, !enable_it);   // LOW is enable
        // Beta joint is using servo.
    } else {
        Serial.print("[Warn] Board_GobotHouse_2206::EnableMotor()  wrong parameter axis_name = ");
        Serial.println(axis_name);
    }
}

// void Board_GobotHouse_2206::SingleMotorMoveTo(char motor_name, float motor_position) {

// }         
