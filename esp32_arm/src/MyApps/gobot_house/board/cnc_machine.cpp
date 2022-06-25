
#include "cnc_machine.h"


void GobotHouseMachine::Init(char solution){
        // [when microstep==1] The value 514.0 is measured/Calibrated from running 10 rounds, Not calculated from anywhere.
        // This value should be calculated from motor's datasheet.  But can not get datasheet for 28-BGY motor.
        // this->motor_steps_per_round = 514 * this->MOTOR_MICRO_STEPS;

        // this->MAX_ACCELERATION_ALPHPA = 600 * this->MOTOR_MICRO_STEPS;
        // this->MAX_STEPS_PER_SECOND_ALPHA = 4000 * this->MOTOR_MICRO_STEPS;    // tested 1200          
        // this->MAX_ACCELERATION_BETA = 200 * this->MOTOR_MICRO_STEPS;
        // this->MAX_STEPS_PER_SECOND_BETA =  500 * this->MOTOR_MICRO_STEPS; 

        // this->Homing_acceleration_alpha = 200 * this->MOTOR_MICRO_STEPS;
        // this->_homing_speed_alpha_degree_per_second = 200 * this->MOTOR_MICRO_STEPS;   //tested 200 * 1
        this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
        this->_HOMING_SPEED[AXIS_ALPHA] = 200.0f;
        
        this->_HOMING_DIR_IS_TO_MAX[AXIS_BETA] = -1;
        this->_HOMING_SPEED[AXIS_BETA] = 200.0f;

        // this->Homing_acceleration_beta = 500 * this->MOTOR_MICRO_STEPS;
        // this->Homing_speed_beta = 60 * this->MOTOR_MICRO_STEPS; 


        // this->_homed_position_alpha_in_degree = 13.6;
        // this->_homed_position_beta_in_degree = 138.0;
        this->_HOMED_POSITION[AXIS_ALPHA] = 13.6;
        this->_HOMED_POSITION[AXIS_BETA] = 138;
        this->PrintOut();
}


float GobotHouseMachine::GetHomingVelocity(char axis_name){
    if (axis_name == 'A')
        // return this->_homing_speed_alpha_degree_per_second * this->HOMING_DIR_ALPHA;
        // return this->HomingSpeed(AXIS_ALPHA) * this->HOMING_DIR_ALPHA;
        return this->HomingSpeed(AXIS_ALPHA) * this->HomingDir_IsToMax(AXIS_ALPHA);
    else if (axis_name == 'B')
        // return this->Homing_speed_beta * this->HOMING_DIR_BETA;
        // return this->Homing_speed_beta * this->HomingDir_IsToMax(AXIS_BETA);
        return this->HomingSpeed(AXIS_BETA) * this->HomingDir_IsToMax(AXIS_BETA);
}

void GobotHouseMachine::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\n   GobotHouseMachine::PrintOut()");
    Serial.print("\n       Homed_position_alpha_in_degree= ");
    Serial.print(RAD_TO_DEG * this->Homed_position_alpha_in_rad);

    Serial.print("\n       Homed_position_beta_in_degree= "); 
    Serial.print(RAD_TO_DEG * this->Homed_position_beta_in_rad);

    // Serial.print("\n   MAX_ACCELERATION_ALPHPA= ");
    // Serial.print(this->MAX_ACCELERATION_ALPHPA);

    Serial.print("\n\n===============================================");
}

