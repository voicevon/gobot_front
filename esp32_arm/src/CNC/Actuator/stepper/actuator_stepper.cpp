#include "actuator_stepper.h"
// #include "CNC/gcode/line_segment_queue.h"   //TODO:  use block


void CncActuatorStepper::LinkStepper(Stepper* stepper, float steps_per_cnc_unit){
    this->_stepper = stepper;
    this->__steps_per_cnc_unit =steps_per_cnc_unit;
}

void CncActuatorStepper::PrintOut(const char* title){
    Logger::Info(title);
    Serial.print("CncActuatorStepper  in");
    Serial.println(title);
    Serial.print("Positions:  current=" );
    // Serial.println(RAD_TO_DEG * this->ConvertPosition_ToCncUnit(_current_position));
    Serial.print("Mechanic: steps_per_unit= ");
    Serial.print(this->__steps_per_cnc_unit);
    // Serial.print("  is_range_constraint= ");
    // Serial.print(this->_is_range_constraint);
    Serial.println(FCBC_RESET);
}



// Must clear to understand:  cnc_position,  actuator_position(or joint_position), and motor_position.
// void CncActuatorStepper::SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit){
void CncActuatorStepper::UpdateMovement(MoveBlock_SingleActuator* move){
    int32_t motor_position_in_step;
    this->_target_position = move->TargetPosition;
    float actuator_position = move->TargetPosition;
    // if (this->_is_range_constraint){
    //     actuator_position = this->_range_constraint->_ConvertTo_ActuatorRange(move->TargetPosition);
    // }
    motor_position_in_step = actuator_position * this->__steps_per_cnc_unit;
    this->_stepper->setTargetAbs(motor_position_in_step);

    bool debug = true;
    if (debug){
        Logger::Info("CncActuatorStepper::SetTargetPositionTo()");
        // Logger::Print("is_absolute_position", move->IsAbsTargetPosition);
        Logger::Print("motor_position_in_step", motor_position_in_step);
        Logger::Print("Target_position: actuator ", RAD_TO_DEG * actuator_position);
        Logger::Print("accelleration ", "in TODO list, currently is default. ");
    }

    debug = true;
    if(debug){
        Logger::Debug(" CncActuatorStepper::MoveTo()" );
        Serial.print("steps_per_cnc_unit= ");
        Serial.print(this->__steps_per_cnc_unit);
        
        // Serial.print("  actuator_speed= ");
        // Serial.println(RAD_TO_DEG * this->GetSpeed());


        Serial.print("Current_position: stepper = ");
        Serial.println(this->_stepper->getPosition());

        // Serial.print("Target position:  is_absolute= ");
        // Serial.print(move->IsAbsTargetPosition);
        // Serial.print(" position: ");
        // Serial.print(RAD_TO_DEG * move->TargetPosition);

        Serial.print(" cnc_position degree= ");
        Serial.print(RAD_TO_DEG * this->_target_position);

        Serial.print("  stepper_position= ");
        Serial.print(motor_position_in_step);
        Serial.println(FCBC_RESET);
    }
}

