#include "actuator_alpha_beta.h"

GobotMain_Actuator_MechanicConfig::GobotMain_Actuator_MechanicConfig(){
    this->_micro_steps_on_stepper_driver = 16;
    this->_motor_gear_teeth_count = 10;
    this->_slave_pulley_teeth_count = 90;
    this->_motor_step_angle_in_degree = 0.7003891050583658;
}
