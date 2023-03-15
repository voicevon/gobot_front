#include "actuator_fast_stepper.h"


void CncActuatorFastStepper::LinkStepper(FastAccelStepper* stepper){
    this->_stepper = stepper;
}

void CncActuatorFastStepper::SpinOnce(){
    // if (_is_moving != _stepper->isRunning()){
    //     Logger::Warn("CncActuatorFastStepper::SpinOnce()") ;
    //     Logger::Print("CncActuatorFastStepper::SpinOnce()  axis name", MyName );
    //     Logger::Print("CncActuatorFastStepper::SpinOnce()  is_moving  update to ", _stepper->isRunning() );
    // }
    _is_moving = _stepper->isRunning();
    // PrintOut("From CncActuatorFastStepper::SpinOnce()", 800000);
}

void CncActuatorFastStepper::PrintOut(const char* title, uint32_t count_up_max){
    static uint32_t counter = 0;
    counter++;
    if (counter >= count_up_max){
        counter = 0;
        Logger::Info(title);
        Logger::Print("distance to target", _stepper->targetPos() - _stepper->getCurrentPosition());
        Logger::Print("current position", _stepper->getCurrentPosition());
        Logger::Print("current speed InMilliHz", _stepper->getSpeedInMilliHz());
        Logger::Print("Accelleration", _stepper->getAcceleration());
    }
}


// Must clear to understand:  cnc_position is very logical concept.
// actuator_position == motor_position for FastStepper here.
void CncActuatorFastStepper::UpdateMovement(MoveBlock_SingleActuator* move){
    _target_position = move->TargetPosition;
    //TODO:  solve this problem in planner.  will keep the below line.
    // _stepper->setSpeedInHz(move->Speed);
    // _stepper->setAcceleration(move->Acceleration);
    // _stepper->applySpeedAcceleration();
    _stepper->enableOutputs();
    _stepper->moveTo(_target_position, false);
    _is_moving = true; 
}


void CncActuatorFastStepper::ForceStop(){
    _stepper->forceStop();
    _is_moving = false;
}

void CncActuatorFastStepper::SetCurrentPositionAs(float new_position){
    _stepper->setCurrentPosition(new_position);
}

float CncActuatorFastStepper::GetCurrentPosition(){
    return _stepper->getCurrentPosition();
}

// void CncActuatorFastStepper::TurnToSleep_WhenIdle(){
//     // _stepper->setAutoEnable(true);
// }
