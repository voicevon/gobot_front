#include "smoker_board.h"


void Smoker_Board::Init(const char* app_welcome_statement){
    _InitSerial(app_welcome_statement);
    Logger::Info("Runnin on board:  Smoker_board:)");

    __all_position_trigger[0].Init('X',PIN_HOME_ALPHA_2201, LOW);
    gs_PositionTrigger_Array::Instance().Init(__all_position_trigger, 1);
}

// void Smoker_Board::SayHello() {
    
// }


// Stepper* Smoker_Board::GetStepper(char axis_name) {
//     if (axis_name=='A'){
//         return &this->stepper_alpha;
//     }else{
//         Serial.print("['Error']  Board_SpringMaker::GetStepper()   axis_name= ");
//         Serial.println(axis_name);
//     }

// }

// SinglePositionTrigger* Smoker_Board::GetSingleHomer(EnumAxis axis_name) {
//     if (axis_name== AXIS_ALPHA){
//         return &this->homer_alpha;
//     }else{
//         Serial.print("['Error']  Board_SpringMaker::GetHomer()   axis_name= ");
//         Serial.println(axis_name);
//     }
//     return nullptr;
// }

// RobotEef_SmokeMachine* Smoker_Board::GetEef() {
//     return &this->eef;
// }


void Smoker_Board::EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) {
    if (axis_name == AXIS_ALPHA){
        digitalWrite(PIN_ALPHA_ENABLE_2201, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] Board_SpringMaker::EnableMotor()  ");
    }
}

