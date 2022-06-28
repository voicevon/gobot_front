#include "mechanic_polor.h"

float ActuatorMechanic_StepperPolor::GetStepsPerCncUnit(){
    // The unit is RAD here.
    float steps_per_motor_round =  360.0f / this->_motor_step_angle_in_degree * this->_micro_steps_on_stepper_driver;
    float gear_ratio = this->_slave_pulley_teeth_count / this->_motor_gear_teeth_count;
    float steps_per_rad =  steps_per_motor_round / TWO_PI * gear_ratio;
    bool debug = false;
    if (debug){
        Logger::PrintTitle("ActuatorMechanic_StepperPolor::GetStepsPerCncUnit() ");
        Serial.print("steps_per_motor_round= ");
        Serial.print(steps_per_motor_round); 
        Serial.print("  gear ratio= ");
        Serial.print(gear_ratio);
        Serial.print("  steps_per_rad= ");
        Serial.println(steps_per_rad);
    }
    return steps_per_rad;
}
