#include "alpha_mechanic.h"


// [when microstep==1] The value 514.0 is measured/Calibrated from running 10 rounds, Not calculated from anywhere.
// This value should be calculated from motor's datasheet.  But can not get datasheet for 28-BGY motor.
// this->motor_steps_per_round = 514 ;

// For alpha:   big_gear_teeth == 90 / small_gear_teeth == 10


GobotMain_AlphaStepper_Mechanic::GobotMain_AlphaStepper_Mechanic(){
    this->_micro_steps_on_stepper_driver = 16;
    this->_motor_gear_teeth_count = 10;
    this->_slave_pulley_teeth_count = 90;
    this->_motor_step_angle_in_degree = 0.7003891050583658;
}