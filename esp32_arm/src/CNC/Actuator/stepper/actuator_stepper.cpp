#include "actuator_stepper.h"


void ActuatorStepper::LinkStepper(Stepper* stepper, float steps_per_cnc_unit){
    this->__stepper = stepper;
    this->__steps_per_cnc_unit =steps_per_cnc_unit;
}


float ActuatorStepper::GetCurrentPosition_InCncUnit(){
    float actuator_position = 1.0f * this->__stepper->getPosition() / this->__steps_per_cnc_unit;
    this->__current_cnc_position_in_rad = actuator_position;
    if (this->_is_range_constraint){
        // convert actuator position in CNC RANGE
        // this->__current_cnc_position_in_rad = this-> _ConvertTo_CncRange(actuator_position);
        this->__current_cnc_position_in_rad = this->_range_constraint->_ConvertTo_CncRange(actuator_position);
    }
    return this->__current_cnc_position_in_rad;
    // Serial.println("[Error] ActuatorStepper::GetCurrentPosition_InCncUnit()  I don't know the anser now!");
    // while(1){
    //     Serial.print("X ");
    //     delay(500);
    // }
}

float ActuatorStepper::GetAbsDistanceToTarget_InCncUnit(){
    int32_t distance_in_step = this->__stepper->getDistanceToTarget();
    bool debug = false;
    if (debug){
        Serial.print("[Debug] ActuatorStepper::GetAbsDistanceToTarget_InCncUnit() ");
        Serial.print(" current_step_position= " );
        Serial.print(this->__stepper->getPosition());
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
        // joint_position = this->_ConvertTo_ActuatorRange(position_in_cnc_unit);
        joint_position = this->_range_constraint->_ConvertTo_ActuatorRange(position_in_cnc_unit);
    }
    int32_t position_in_step = joint_position * this->__steps_per_cnc_unit;
    this->__stepper->setPosition(position_in_step);

    bool debug = true;
    if(debug){
        Serial.print("[Debug] ActuatorStepper::SetPosition() __steps_per_cnc_unit= ");
        Serial.print(this->__steps_per_cnc_unit);
        Serial.print("  cnc position in degree = ");
        Serial.print(RAD_TO_DEG * position_in_cnc_unit);
        Serial.print("  Stepper position= ");
        Serial.println(position_in_step);
    }
    this->__current_cnc_position_in_rad = position_in_cnc_unit;  //??

}

// Must clear to understand:  cnc_position,  actuator_position(or joint_position), and motor_position.
void ActuatorStepper::SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit){
    // int32_t posititon_in_step = position_in_cnc_unit * this->__steps_per_cnc_unit;
    int32_t motor_position_in_step;
    if (is_absolute_position){
        this->_target_cnc_position = position_in_cnc_unit;
        float actuator_position = position_in_cnc_unit;
        if (this->_is_range_constraint){
            // joint_position = this->_ConvertTo_ActuatorRange(position_in_cnc_unit);
            actuator_position = this->_range_constraint->_ConvertTo_ActuatorRange(position_in_cnc_unit);
        }
        motor_position_in_step = actuator_position * this->__steps_per_cnc_unit;
        this->__stepper->setTargetAbs(motor_position_in_step);

        bool debug = false;
        if (debug){
            Serial.print("[Debug] ActuatorStepper::MoveTo( absolute)  position_in_cnc_unit= ");
            Serial.print(RAD_TO_DEG * position_in_cnc_unit);
            Serial.print("  actuator_position= ");
            Serial.print(RAD_TO_DEG * actuator_position);
            Serial.print(" motor_position_in_step= ");
            Serial.println (motor_position_in_step);
            Serial.print(" motor_speed= ");
            Serial.print(RAD_TO_DEG * this->GetSpeed());
            Serial.print(" accelleration= ");
            Serial.println( "in TODO list, currently is default. ");
        }
    } else {
        this->_target_cnc_position += position_in_cnc_unit;
        motor_position_in_step = position_in_cnc_unit * this->__steps_per_cnc_unit;
        this->__stepper->setTargetRel(motor_position_in_step);
    }
    this->__distance_to_target = abs(this->_target_cnc_position - this->__current_cnc_position_in_rad);

    bool debug = false;
    if(debug){
        Serial.print("[Debug] ActuatorStepper::MoveTo() steps_per_cnc_unit=");
        Serial.print(this->__steps_per_cnc_unit);
        Serial.print("  stepper_position= ");
        Serial.print(motor_position_in_step);
        Serial.print("  _target_cnc_position degree= ");
        Serial.println(RAD_TO_DEG * this->_target_cnc_position);
    }
}

#define MAX_STEPS_PER_SECOND 6000   //TODO:  be configable
void ActuatorStepper::SetSpeed(float speed_per_second){
    float steps_per_second = speed_per_second * this->__steps_per_cnc_unit;
    if (steps_per_second > MAX_STEPS_PER_SECOND){
        steps_per_second = MAX_STEPS_PER_SECOND;
    }
    this->__stepper->setMaxSpeed(steps_per_second);
    // the real speed of actuator.
    this->__speed = steps_per_second / this->__steps_per_cnc_unit;
    bool debug = false;
    if (debug){
        Serial.print("[Debug] ActuatorStepper::SetSpeed() speed= ");
        Serial.println(RAD_TO_DEG * this->__speed);
    }
}

