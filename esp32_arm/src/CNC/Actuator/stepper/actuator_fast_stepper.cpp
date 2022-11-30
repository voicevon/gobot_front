#include "actuator_fast_stepper.h"


void CncActuatorFastStepper::LinkStepper(FastAccelStepper* stepper, float steps_per_cnc_unit){
    this->_stepper = stepper;
    this->__steps_per_cnc_unit =steps_per_cnc_unit;
}

void CncActuatorFastStepper::PrintOut(const char* title){
    Logger::Info(title);
    Serial.print("CncActuatorFastStepper  in");
    Serial.println(title);
    Serial.print("Positions:  current=" );
    Serial.println(RAD_TO_DEG * this->ConvertPosition_ToCncUnit(_current_position));
    Serial.print("Mechanic: steps_per_unit= ");
    Serial.print(this->__steps_per_cnc_unit);
    Serial.print("  is_range_constraint= ");
    Serial.print(this->_is_range_constraint);
    Serial.println(FCBC_RESET);
}


// Must clear to understand:  cnc_position,  actuator_position(or joint_position), and motor_position.
void CncActuatorFastStepper::UpdateMovement(MoveBlock_SingleActuator* move){
    int32_t motor_position_in_step;
    // if (move->IsAbsTargetPosition){
    this->_target_position = move->TargetPosition;
    this->_stepper->setSpeedInHz(move->Speed);
    this->_stepper->setAcceleration(move->Acceleration);
    this->_stepper->applySpeedAcceleration();
    float actuator_position = move->TargetPosition;
    if (this->_is_range_constraint){
        actuator_position = this->_range_constraint->_ConvertTo_ActuatorRange(move->TargetPosition);
    }
    motor_position_in_step = actuator_position * this->__steps_per_cnc_unit;
    // this->_stepper->setPositionAfterCommandsCompleted(motor_position_in_step);
    _stepper->enableOutputs();
    _stepper->moveTo(motor_position_in_step, false);

    bool debug = false;
    if (debug){
        Logger::Info("CncActuatorFastStepper::UpdateMovement()");
        Logger::Print("motor_position_in_step", motor_position_in_step);
        Logger::Print("Target_position: actuator ", RAD_TO_DEG * actuator_position);
        Logger::Print("accelleration ", move->Acceleration);
        }

    debug = false;
    if(debug){
        Logger::Debug(" CncActuatorFastStepper::UpdateMovement()" );
        Logger::Print("steps_per_cnc_unit= ", this->__steps_per_cnc_unit);
        Logger::Print("Current_position: stepper ", this->_stepper->getCurrentPosition());
        Logger::Print("cnc_position degree ", RAD_TO_DEG * this->_target_position);
        Logger::Print("stepper_position= ", motor_position_in_step);
    }
}


void CncActuatorFastStepper::ForceStop(){
    _stepper->forceStop();
}
