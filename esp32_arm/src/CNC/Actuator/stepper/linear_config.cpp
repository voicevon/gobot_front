#include "linear_config.h"

float ActuatorStepper_LinearConfig::GetStepsPerCncUnit(){
    // The unit is mm here.
    float steps_per_motor_round =  360.0f / this->_motor_step_angle_in_degree * this->_micro_steps_on_stepper_driver;
    float steps_per_mm_of_motor_pulley = this->_motor_gear_pitch_in_mm * this->_motor_gear_teeth_count / steps_per_motor_round;
    float gear_ratio = this->_slave_pulley_teeth_count / this->_motor_gear_teeth_count;
    float steps_of_output_pulley_per_mm = steps_per_mm_of_motor_pulley * gear_ratio;  //?? something is wrong here !!!
    return steps_of_output_pulley_per_mm;
}
