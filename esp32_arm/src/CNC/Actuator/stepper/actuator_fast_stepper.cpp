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
}

void CncActuatorFastStepper::PrintOut(const char* title){
    Logger::Info(title);
    Serial.print("CncActuatorFastStepper  in");
    Serial.println(title);
    Serial.print("Positions:  current=" );
    // Serial.println(RAD_TO_DEG * this->ConvertPosition_ToCncUnit(_current_position));
    Serial.print("  is_range_constraint= ");
    // Serial.print(this->_is_range_constraint);
    Serial.println(FCBC_RESET);
}


// Must clear to understand:  cnc_position,  actuator_position == motor_position.
void CncActuatorFastStepper::UpdateMovement(MoveBlock_SingleActuator* move){
    this->_target_position = move->TargetPosition;
    this->_stepper->setSpeedInHz(move->Speed);
    this->_stepper->setAcceleration(move->Acceleration);
    this->_stepper->applySpeedAcceleration();
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
