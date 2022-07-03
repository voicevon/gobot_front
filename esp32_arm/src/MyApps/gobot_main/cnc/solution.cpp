#include "solution.h"
#include "../cnc/actuator_alpha_beta.h"
#include "mover.h"

void GobotMainCnc::Init(CncBoardBase* board){
    this->_Init_FiverBarBase(board);
    this->_LinkEef(board->GetEef());
    Board_GobotMain* my_board = (Board_GobotMain*) board;
    ActuatorStepper* alpha = (ActuatorStepper*)my_board->GetActuator(AXIS_ALPHA);
    this->__mover.LinkStepper_asAlpha(alpha);
    ActuatorStepper* beta = (ActuatorStepper*)my_board->GetActuator(AXIS_BETA);
    this->__mover.LinkStepper_asBeta(beta);
    this->__SelfTest();
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
