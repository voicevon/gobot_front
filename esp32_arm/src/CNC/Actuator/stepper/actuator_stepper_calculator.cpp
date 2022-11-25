#include "actuator_stepper_calculator.h"


float ActuatorStepper_Calculator::Get_Formular_Slope_raw_per_rad(){
    // The unit is RAD here.
    float steps_per_motor_round =  360.0f / this->motor_step_angle_in_degree * this->motor_driver_micro_steps;
    float gear_ratio = this->slave_pulley_teeth_count / this->motor_gear_teeth_count;
    float steps_per_rad =  steps_per_motor_round / TWO_PI * gear_ratio;
    bool debug = false;
    if (debug){
        Logger::Info("ActuatorStepper_PolorConfig::GetStepsPerCncUnit() ");
        Serial.print("steps_per_motor_round= ");
        Serial.print(steps_per_motor_round); 
        Serial.print("  gear ratio= ");
        Serial.print(gear_ratio);
        Serial.print("  steps_per_rad= ");
        Serial.println(steps_per_rad);
    }
    return steps_per_rad;
}


float ActuatorStepper_Calculator::Get_Formular_Slope_raw_per_mm(){
    // The unit is mm here.
    float steps_per_motor_round =  360.0f / this->motor_step_angle_in_degree * this->motor_driver_micro_steps;
    float steps_per_mm_of_motor_pulley = this->motor_gear_pitch_in_mm * this->motor_gear_teeth_count / steps_per_motor_round;
    float gear_ratio = this->slave_pulley_teeth_count / this->motor_gear_teeth_count;
    float steps_of_output_pulley_per_mm = steps_per_mm_of_motor_pulley * gear_ratio;  //?? something is wrong here !!!
    return steps_of_output_pulley_per_mm;
}

