#include "board.h"



void GobotMain_Board::InitHardware(){
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

    // this->__alpha_stepper.setInverseRotation(false);
    // this->__beta_stepper.setInverseRotation(false);
}

void GobotMain_Board::Init(){
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n\n");
    Logger::Info("==========================================");
    Logger::Info("Hi Xuming, I am GobotMain. Good luck......");
    Logger::Info("==========================================");
    Serial.print(FCBC_RESET);
    this->InitHardware();

    //Init CNC

    // this->__actuator_alpha.LinkStepper(this->__alpha_stepper, );
    // this->__actuator_beta.LinkStepper(this->__beta_stepper);
    this->__eef.Init();

    __all_position_triggers[0].Init('a',PIN_HOME_ALPHA_2201, LOW);
    __all_position_triggers[1].Init('b',PIN_HOME_BETA_2201, LOW);
    gs_PositionTrigger_Array::Instance().Init(__all_position_triggers,2);

    this->RepportRamUsage();
}


// Stepper* GobotMain_Board::GetStepper(EnumAxis_Inverseinematic axis){
FastAccelStepper* GobotMain_Board::GetStepper(EnumAxis_Inverseinematic axis){
    if (axis==AXIS_ALPHA){
        return &this->__alpha_stepper;
    }else if (axis==AXIS_BETA){
        return &this->__beta_stepper;
    }else{
        Logger::Halt(" GobotMain_Board::GetStepper()   axis_name= ");
        Serial.println(axis);
    }
    return nullptr;
}

// CncActuatorBase* GobotMain_Board::GetActuator(EnumAxis axis) {
//     if (axis==AXIS_ALPHA){
//         return &this->__actuator_alpha;
//     }else if (axis==AXIS_BETA){
//         return &this->__actuator_beta;
//     }else{
//         Logger::Halt(" GobotMain_Board::GetStepper()   axis_name= ");
//         Serial.println(axis);
//     }
//     return nullptr;
// }

// SinglePositionTrigger* GobotMain_Board::GetSingleHomer(EnumAxis axis) {
//     if (axis == AXIS_ALPHA){
//         return &this->homer_alpha;
//     }else if (axis == AXIS_BETA){
//         return &this->homer_beta;
//     }else{
//         Logger::Halt("GobotMain_Board::GetHomer()   axis= ");
//         // Serial.println(axis);
//     }
//     return nullptr;
// }

// AxisHomer* GobotMain_Board::GetPositionTriggers(EnumAxis axis_name){
//     Logger::Error("GobotMain_Board::GetPositionTriggers() NOT SUPPORT");
// }

// RobotEef_GobotMain* GobotMain_Board::GetEef() {
//     return &this->__eef;
// }




void GobotMain_Board::EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) {
    bool debug = false;
    if(debug){
        Serial.print("[Info] GobotMain_Board::EnableMotor()  axis_name= ");
        Serial.print(axis_name);
        Serial.print("\t enable = ");
        Serial.println(enable_it);
    }
    if (axis_name == AXIS_ALPHA){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else if (axis_name == AXIS_BETA){
        digitalWrite(PIN_BETA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] GobotMain_Board::EnableMotor() axis =");
        Serial.println(axis_name);
    }
}

uint8_t GobotMain_Board::ReadAllRooms(){
    uint8_t result = 0;
    uint8_t p;
    for (int i=0; i<8; i++){
        p = digitalRead(PIN_ROOMS[i]);
        result += (p<<i);
    }
    return result;
}


//Return 9 is all rooms is empty.
uint8_t GobotMain_Board::GetLoadedRoom(){
    uint8_t v;
    for(uint8_t i=0; i<8; i++){
        v = digitalRead(PIN_ROOMS[i]);
        if(v == LOW)
            return i;
    }
    return 9;   
}
