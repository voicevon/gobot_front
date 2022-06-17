#include "mechanic_polor.h"

uint32_t ActuatorMechanic_StepperPolor::GetStepsPerCncUnit(){
    // The unit is RAD here.
    float steps_per_motor_round =  360.0f / this->_motor_step_angle_in_degree * this->_micro_steps_on_stepper_driver;
    return steps_per_motor_round / TWO_PI;
}
