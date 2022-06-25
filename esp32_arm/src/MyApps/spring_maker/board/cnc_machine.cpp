#include "cnc_machine.h"


// int32_t SpringMakerMachine::GetStepsPerUnit(char axis_name) {
//     return 100;
// }

SpringMakerMachine::SpringMakerMachine(){
        // this->STEPPER_DRIVER_MICRO_STEPS = 16;
        // this->MOTOR_GEARBOX_RATIO_ALPHA = 51;
        // this->MOTOR_DEGREE_PER_STEP = 1.8;
        // this->max_acceleration_alpha = 6000;
        // this->max_speed_alpha = 20000;

        // this->motor_steps_per_round = 1.0 * this->STEPPER_DRIVER_MICRO_STEPS;
        // this->Homing_speed_alpha = 13000;
        this->_HOMING_SPEED[AXIS_ALPHA] = 13000.0f;
        this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
        this->_HOMING_ACCELERATION[AXIS_ALPHA] = 6000;
        // this->Homing_acceleration_alpha = 6000;

        // this->steps_per_mm_for_z = 200;
        // this->steps_per_rad_for_a = 360.0f * this->STEPPER_DRIVER_MICRO_STEPS  * this->MOTOR_GEARBOX_RATIO_ALPHA / this->MOTOR_DEGREE_PER_STEP / TWO_PI;
}

void SpringMakerMachine::PrintOut(const char * title){
    Serial.print("\n\n===============================================");
    Serial.print("\nSpringMakerMachine::PrintOut()");
    // Serial.print("\n   STEPPER_DRIVER_MICRO_STEPS = ");
    // Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);
    // Serial.print("\n   MOTOR_DEGREE_PER_STEP = ");
    // Serial.print(this->MOTOR_DEGREE_PER_STEP);
    // Serial.print("\n   MOTOR_GEARBOX_RATIO_ALPHA =");
    // Serial.print(this->MOTOR_GEARBOX_RATIO_ALPHA);

    // Serial.print("\n    steps_per_rad_for_a = ");
    // Serial.print(this->steps_per_rad_for_a);
    Serial.print("\n     Home_position (a) = (");
    Serial.print(this->HomedPosition(AXIS_ALPHA));
    Serial.print(" , ");
    Serial.print("\n     Homing_speed_alpha = ");
    Serial.print(this->HomingSpeed(AXIS_ALPHA));
    // Serial.print("\n     Homing_acceleration_alpha = ");
    // Serial.print(this->Homing_acceleration_alpha);
    // Serial.print("\n\n        max_speed_alpha = ");
    // Serial.print(max_acceleration_alpha);


    Serial.print("\n----------------------------------------------\n\n");
}

