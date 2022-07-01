#include "board.h"
#include "../cnc/actuator_alpha_beta.h"



void Board_GobotMain::InitHardware(){
    // init gpio.
    pinMode(PIN_ALPHA_ENABLE_2201, OUTPUT);
    pinMode(PIN_BETA_ENABLE_2201, OUTPUT);

    this->PIN_ROOMS[0] = PIN_SENSOR_ROOM_0;
    this->PIN_ROOMS[1] = PIN_SENSOR_ROOM_1;
    this->PIN_ROOMS[2] = PIN_SENSOR_ROOM_2;
    this->PIN_ROOMS[3] = PIN_SENSOR_ROOM_3;
    this->PIN_ROOMS[4] = PIN_SENSOR_ROOM_4;
    this->PIN_ROOMS[5] = PIN_SENSOR_ROOM_5;
    this->PIN_ROOMS[6] = PIN_SENSOR_ROOM_6;
    this->PIN_ROOMS[7] = PIN_SENSOR_ROOM_7;
    for(int i=0;i<8;i++){
        pinMode(this->PIN_ROOMS[i], INPUT_PULLUP);
    }

    GobotMain_ActuatorAlphaBetaMechanic alpha;
    this->__alpha_stepper.setInverseRotation(false);
    this->__actuator_alpha.LinkStepper(&this->__alpha_stepper, alpha.GetStepsPerCncUnit());

    GobotMain_ActuatorAlphaBetaMechanic beta;
    this->__beta_stepper.setInverseRotation(true);
    this->__actuator_beta.LinkStepper(&this->__beta_stepper, beta.GetStepsPerCncUnit());
}

void Board_GobotMain::Init(bool is_on_reset){
    Serial.begin(115200);
    Logger::Info("===================================================");
    Logger::Info("Hi Xuming, I am GobotMain. Good luck......");
    Serial.print(FCBC_RESET);
    this->InitHardware();
    this->EnableMotor(AXIS_ALPHA, false);
    this->EnableMotor(AXIS_BETA, false);

    this->__eef.Init();

    this->__mover_dual_step.LinkStepper_asAlpha(&this->__actuator_alpha);
    this->__mover_dual_step.LinkStepper_asBeta(&this->__actuator_beta);
    this->cnc_mover = &this->__mover_dual_step;

    this->RepportRamUsage();
}

void Board_GobotMain::PrintOut(){
    this->cnc_mover->PrintOut("Board_GobotMain. __cnc_mover");
    this->__cnc_config.PrintOut("Board_GobotMain.__cnc_config");
    this->__actuator_alpha.PrintOut("Board_GobotMain. __actuator_alpha");
    this->__actuator_beta.PrintOut("Board_GobotMain. __actuator_beta");

}

ActuatorBase* Board_GobotMain::GetActuator(EnumAxis axis_name) {
    if (axis_name==AXIS_ALPHA){
        return &this->__actuator_alpha;
    }else if (axis_name==AXIS_BETA){
        return &this->__actuator_beta;
    }else{
        Serial.print("['Error']  Board_GobotMain::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
}

SingleAxisHomer* Board_GobotMain::GetHomer(EnumAxis axis_name) {
    if (axis_name==AXIS_ALPHA){
        return &this->homer_alpha;
    }else if (axis_name==AXIS_BETA){
        return &this->homer_beta;
    }else{
        Serial.print("['Error']  Board_GobotMain::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
    return nullptr;
}

RobotEef_GobotMain* Board_GobotMain::GetEef() {
    return &this->__eef;
}




void Board_GobotMain::EnableMotor(EnumAxis axis_name, bool enable_it) {
    bool debug = false;
    if(debug){
        Serial.print("[Info] Board_GobotMain::EnableMotor()  axis_name= ");
        Serial.print(axis_name);
        Serial.print("\t enable = ");
        Serial.println(enable_it);
    }
    if (axis_name == AXIS_ALPHA){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else if (axis_name == AXIS_BETA){
        digitalWrite(PIN_BETA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_GobotMain::EnableMotor() axis =");
        Serial.println(axis_name);
    }
}

uint8_t Board_GobotMain::ReadAllRooms(){
    uint8_t result = 0;
    uint8_t p;
    for (int i=0; i<8; i++){
        p = digitalRead(PIN_ROOMS[i]);
        result += (p<<i);
    }
    return result;
}


//Return 9 is all rooms is empty.
uint8_t Board_GobotMain::GetLoadedRoom(){
    uint8_t v;
    for(uint8_t i=0; i<8; i++){
        v = digitalRead(PIN_ROOMS[i]);
        if(v == LOW)
            return i;
    }
    return 9;   
}
