#include "alpha_stepper.h"

GobotHouse2206_AlphaStepper_Mechanic::GobotHouse2206_AlphaStepper_Mechanic(){
    this->_micro_steps_on_stepper_driver = 16;
    this->_motor_gear_teeth_count = 10;
    this->_motor_step_angle_in_degree = 1.222;   // ?
}