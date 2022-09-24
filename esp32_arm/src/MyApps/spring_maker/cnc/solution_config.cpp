#include "solution_config.h"



SpringMaker_CncSolutionConfig::SpringMaker_CncSolutionConfig(){
    HomingConfig* config = this->GetAxisHomers()->GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    config->IsDirectionToMax = true;
    config->Accelleration = 6000;
    config->Speed = 13000.0f;
        // this->_HOMING_SPEED[AXIS_ALPHA] = 13000.0f;
        // this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
        // this->_HOMING_ACCELERATION[AXIS_ALPHA] = 6000;
}

void SpringMaker_CncSolutionConfig::PrintOut(const char * title){
    Serial.print("\n\n===============================================");
    Serial.print("\nSpringMaker_CncSolutionConfig::PrintOut()");
    // Serial.print("\n   STEPPER_DRIVER_MICRO_STEPS = ");
    // Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);
    // Serial.print("\n   MOTOR_DEGREE_PER_STEP = ");
    // Serial.print(this->MOTOR_DEGREE_PER_STEP);
    // Serial.print("\n   MOTOR_GEARBOX_RATIO_ALPHA =");
    // Serial.print(this->MOTOR_GEARBOX_RATIO_ALPHA);

    // Serial.print("\n    steps_per_rad_for_a = ");
    // Serial.print(this->steps_per_rad_for_a);
    Serial.print("\n     Home_position (a) = (");
    // Serial.print(this->HomedPosition(AXIS_ALPHA));
    Serial.print(" , ");
    Serial.print("\n     Homing_speed_alpha = ");
    // Serial.print(this->HomingSpeed(AXIS_ALPHA));
    // Serial.print("\n     Homing_acceleration_alpha = ");
    // Serial.print(this->Homing_acceleration_alpha);
    // Serial.print("\n\n        max_speed_alpha = ");
    // Serial.print(max_acceleration_alpha);


    Serial.print("\n----------------------------------------------\n\n");
}

