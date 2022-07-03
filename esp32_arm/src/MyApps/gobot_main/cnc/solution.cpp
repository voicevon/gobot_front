#include "solution.h"
#include "../cnc/actuator_alpha_beta.h"
#include "mover.h"

void GobotMainCnc::Init(CncBoardBase* board){
    this->__mover.Init(board, nullptr);
    this->_Init_FiverBarBase(board);
    this->_LinkEef(board->GetEef());
    this->_config = &this->__config;
    this->_config_base = &this->__config;

    Logger::Debug("11111111111111111");
    Board_GobotMain* my_board = (Board_GobotMain*) board;
    Logger::Debug("2222222");

    // ActuatorStepper* alpha = (ActuatorStepper*)my_board->GetActuator(AXIS_ALPHA);
    // Stepper* alpha_stepper = my_board->GetStepper(AXIS_ALPHA);
    // alpha->LinkStepper(alpha_stepper);
    Logger::Debug("fffffffffff");
    // this->__mover.LinkStepper_asAlpha(alpha);
    Logger::Debug("33333");
    // ActuatorStepper* beta = (ActuatorStepper*)my_board->GetActuator(AXIS_BETA);
    // this->__mover.LinkStepper_asBeta(beta);
    this->__SelfTest();
    Logger::Debug("44444444");
}

void GobotMainCnc::__SelfTest(){
    this->_mover->PrintOut("GobotMainCnc::__SelfTest()");
}

EnumAxis GobotMainCnc::ConvertToEnum(char axis){
    switch (axis){
        case 'A':
            return AXIS_ALPHA;
            break;
        case 'B':
            return AXIS_BETA;
            break;
        case 'X':
            return AXIS_X;
            break;
        case 'Y':
            return AXIS_Y;
            break;
        default:
            Serial.print("[Error] GobotMainCnc::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            break;
    }
    return AXIS_ALPHA;
}
