#include "mover.h"
#include "actuator_alpha_beta.h"

void GobotMain_CncMover::Init(BoardBase* board, StepControl* stepControl){
    GobotMain_Actuator_MechanicConfig config;
    GobotMain_Board* myboard = (GobotMain_Board*) board;
    this->__alpha.LinkStepper(myboard->GetStepper(AXIS_ALPHA), config.GetStepsPerCncUnit());
    this->LinkStepper_asAlpha(&this->__alpha);

    this->__beta.LinkStepper(myboard->GetStepper(AXIS_BETA), config.GetStepsPerCncUnit());
    this->LinkStepper_asBeta(&this->__beta);
    this->LinkStepControl(stepControl);
}
