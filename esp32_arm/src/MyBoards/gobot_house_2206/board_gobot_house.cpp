#include "board_gobot_house.h"
// #include "mechanic/alpha_stepper.h"
#include "CNC/Actuator/stepper/mechanic_polor.h"
#include "CNC/solution/cnc_scara/cnc_scara.h"


void Board_GobotHouse_2206::__Init_Hardware(){
    //Init GPIO
    pinMode(PIN_ALPHA_ENABLE_2109, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2109, OUTPUT);
    digitalWrite(PIN_BETA_ENABLE_2109, HIGH);   // To avoid A4988 is enabled.  sometimes there is an A4988 chip on the board.
    // Init Actuator alpha
    ActuatorMechanic_StepperPolor alpha;
    alpha._micro_steps_on_stepper_driver = 16;
    alpha._motor_gear_teeth_count = 10;
    alpha._slave_pulley_teeth_count = 90;
    alpha._motor_step_angle_in_degree = 0.7003891050583658;
    this->__stepper_alpha.setInverseRotation(true);
    this->__actuator_alpha.LinkStepper(&this->__stepper_alpha,alpha.GetStepsPerCncUnit());

    // Init actuator beta
	this->__servo_beta.setPeriodHertz(50);
    this->__servo_beta.attach(PIN_CNC_BETA_SERVO);
    this->__actuator_beta.LinkServo(&this->__servo_beta, true);

    // Init mechanic
    this->__cnc_mechanic.Homed_position_alpha_in_degree = 13.6;
    this->__cnc_mechanic.Homed_position_beta_in_degree = 148;
    this->__cnc_mechanic.LINK_A = 75.0;
    this->__cnc_mechanic.LINK_B = 75.0;
    this->__cnc_mechanic.Homing_acceleration_alpha = 500;
    
}

void Board_GobotHouse_2206::Init(bool is_on_reset){
    Serial.begin(115200);
    Serial.println("[Info] Board_GobotHouse_2206::Init()  Hi Xuming, I am your lovely bot,  GobotHouse. ");

    this->__Init_Hardware();
    this->EnableMotor('A', false);

    this->eef.Init();
    this->eef.Run(EEF_CODE_UNLOAD);

    this->mover_StepperServo.LinkStepper_asAlpha(&this->__actuator_alpha);
    this->mover_StepperServo.LinkServo_asBeta(&this->__actuator_beta);
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
    return nullptr;
}

ActuatorServo* Board_GobotHouse_2206::GetActuaorBeta(char axis_name){
    if (axis_name=='B'){
        return &this->__actuator_beta;
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
    return nullptr;
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
