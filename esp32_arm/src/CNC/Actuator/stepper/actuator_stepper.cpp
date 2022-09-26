#include "actuator_stepper.h"
#include "CNC/gcode/line_segment_queue.h"   //TODO:  use block


void ActuatorStepper::LinkStepper(Stepper* stepper, float steps_per_cnc_unit){
    this->_stepper = stepper;
    this->__steps_per_cnc_unit =steps_per_cnc_unit;
}

void ActuatorStepper::PrintOut(const char* title){
    Logger::Info(title);
    Serial.print("ActuatorStepper  in");
    Serial.println(title);
    Serial.print("Positions:  current=" );
    Serial.println(RAD_TO_DEG * this->__current_cnc_position_in_rad);
    Serial.print("Mechanic: steps_per_unit= ");
    Serial.print(this->__steps_per_cnc_unit);
    Serial.print("  is_range_constraint= ");
    Serial.print(this->_is_range_constraint);
    Serial.println(FCBC_RESET);
}

float ActuatorStepper::GetCurrentPosition(){
    float actuator_position = 1.0f * this->_stepper->getPosition() / this->__steps_per_cnc_unit;
    this->__current_cnc_position_in_rad = actuator_position;
    if (this->_is_range_constraint){
        // convert actuator position in CNC RANGE
        // this->__current_cnc_position_in_rad = this-> _ConvertTo_CncRange(actuator_position);
        this->__current_cnc_position_in_rad = this->_range_constraint->_ConvertTo_CncRange(actuator_position);
    }
    return this->__current_cnc_position_in_rad;
    // Serial.println("[Error] ActuatorStepper::GetCurrentPosition()  I don't know the anser now!");
    // while(1){
    //     Serial.print("X ");
    //     delay(500);
    // }
}

float ActuatorStepper::GetAbsDistanceToTarget_InCncUnit(){
    int32_t distance_in_step = this->_stepper->getDistanceToTarget();
    bool debug = false;
    if (debug){
        Serial.print("[Debug] ActuatorStepper::GetAbsDistanceToTarget_InCncUnit() ");
        Serial.print(" current_step_position= " );
        Serial.print(this->_stepper->getPosition());
        Serial.print(" distance to target= " );
        Serial.println(distance_in_step);
    }
    if(distance_in_step < 0){
        Serial.println("[Error] ActuatorStepper::GetAbsDistanceToTarget_InCncUnit() ,  Do inspection! ");
        while(true){
            Serial.print("E ");
            delay(1000);
        };
    }
    return 1.0f * distance_in_step / this->__steps_per_cnc_unit;
}



void ActuatorStepper::SetCurrentPositionAs(float position_in_cnc_unit){
    //TODO:  direction is here.  The joint has range limitation.  --> DONE.  
    float joint_position = position_in_cnc_unit;
    if(this->_is_range_constraint) {
        joint_position = this->_range_constraint->_ConvertTo_ActuatorRange(position_in_cnc_unit);
    }
    int32_t position_in_step = joint_position * this->__steps_per_cnc_unit;
    this->_stepper->setPosition(position_in_step);

    bool debug = true;
    if(debug){
        Logger::Debug("ActuatorStepper::SetCurrentPositionAs()  ");
        Serial.print("__steps_per_cnc_unit= ");
        Serial.print(this->__steps_per_cnc_unit);
        Serial.print("  cnc position in degree = ");
        Serial.print(RAD_TO_DEG * position_in_cnc_unit);
        Serial.print("  Stepper position= ");
        Serial.println(position_in_step);
    }
    this->__current_cnc_position_in_rad = position_in_cnc_unit;  //??

}

// Must clear to understand:  cnc_position,  actuator_position(or joint_position), and motor_position.
// void ActuatorStepper::SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit){
void ActuatorStepper::UpdateMovement(LineSegment* move){
    int32_t motor_position_in_step;
    // if (is_absolute_position){
    if (move->IsAbsTargetPosition){
        // this->_target_cnc_position = position_in_cnc_unit;
        this->_target_cnc_position = move->TargetPosition;
        // float actuator_position = position_in_cnc_unit;
        float actuator_position = move->TargetPosition;
        if (this->_is_range_constraint){
            // actuator_position = this->_range_constraint->_ConvertTo_ActuatorRange(position_in_cnc_unit);
            actuator_position = this->_range_constraint->_ConvertTo_ActuatorRange(move->TargetPosition);
        }
        motor_position_in_step = actuator_position * this->__steps_per_cnc_unit;
        this->_stepper->setTargetAbs(motor_position_in_step);

        bool debug = true;
        if (debug){
            Logger::Info("ActuatorStepper::SetTargetPositionTo()");
            Logger::Print("is_absolute_position", move->IsAbsTargetPosition);
            Logger::Print("motor_position_in_step", motor_position_in_step);
            Logger::Print("Target_position: actuator ", RAD_TO_DEG * actuator_position);
            Logger::Print("accelleration ", "in TODO list, currently is default. ");
        }
    } else {
        // set relative position.
        // this->_target_cnc_position += position_in_cnc_unit;
        this->_target_cnc_position += move->TargetPosition;
        // motor_position_in_step = position_in_cnc_unit * this->__steps_per_cnc_unit;
        motor_position_in_step = move->TargetPosition * this->__steps_per_cnc_unit;
        this->_stepper->setTargetRel(motor_position_in_step);
    }
    this->__distance_to_target = abs(this->_target_cnc_position - this->__current_cnc_position_in_rad);

    bool debug = true;
    if(debug){
        Logger::Debug(" ActuatorStepper::MoveTo()" );
        Serial.print("steps_per_cnc_unit= ");
        Serial.print(this->__steps_per_cnc_unit);
        
        Serial.print("  actuator_speed= ");
        Serial.println(RAD_TO_DEG * this->GetSpeed());


        Serial.print("Current_position: stepper = ");
        Serial.println(this->_stepper->getPosition());

        Serial.print("Target position:  is_absolute= ");
        Serial.print(move->IsAbsTargetPosition);
        Serial.print(" position: ");
        Serial.print(RAD_TO_DEG * move->TargetPosition);

        Serial.print(" cnc_position degree= ");
        Serial.print(RAD_TO_DEG * this->_target_cnc_position);

        Serial.print("  stepper_position= ");
        Serial.print(motor_position_in_step);
        Serial.println(FCBC_RESET);
    }
}

#define MAX_STEPS_PER_SECOND 6000   //TODO:  be configable
void ActuatorStepper::SetSpeed(float speed_in_cnc_unit){
    float steps_per_second = speed_in_cnc_unit * this->__steps_per_cnc_unit;
    if (steps_per_second > MAX_STEPS_PER_SECOND){
        steps_per_second = MAX_STEPS_PER_SECOND;
    }
    this->_stepper->setMaxSpeed(steps_per_second);
    // the real speed of actuator.
    this->__speed = steps_per_second / this->__steps_per_cnc_unit;
    bool debug = false;
    if (debug){
        Serial.print("[Debug] ActuatorStepper::SetSpeed() speed= ");
        Serial.println(RAD_TO_DEG * this->__speed);
    }
}

void ActuatorStepper::SetAccelleration(float new_acceleration){
    int32_t step_acceleration = new_acceleration * this->__steps_per_cnc_unit;
    this->GetLinkedStepper()->setAcceleration(step_acceleration);
}

