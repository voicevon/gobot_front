
// #include "cnc_machine.h"
#include "solution_config_2205.h"


GobotHouseMachine::GobotHouseMachine(){
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




        // this->_homed_position_alpha_in_degree = 13.6;
        // this->_homed_position_beta_in_degree = 138.0;
        this->_HOMED_POSITION[AXIS_ALPHA] = 13.6;
        this->_HOMED_POSITION[AXIS_BETA] = 138;
        this->PrintOut("GobotHouseMachine");
}




void GobotHouseMachine::PrintOut(const char* title){

    Serial.print("\n\n===============================================");
    Serial.print("\n   GobotHouseMachine::PrintOut()");
    Serial.print("\n       Homed_position_alpha_in_degree= ");
    Serial.print(RAD_TO_DEG * this->Homed_position_alpha_in_rad);

    Serial.print("\n       Homed_position_beta_in_degree= "); 
    Serial.print(RAD_TO_DEG * this->Homed_position_beta_in_rad);


    Serial.print("\n\n===============================================");
}

