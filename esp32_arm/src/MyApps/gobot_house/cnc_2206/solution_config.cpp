

#include "solution_config.h"

GobotHouse_CncSolutionConfig_2206::GobotHouse_CncSolutionConfig_2206(){
    this->__InitStaticConfig();

    // this->MAX_ACCELERATION_ALPHPA = 600 ;

    this->_HOMING_SPEED[AXIS_ALPHA] = DEG_TO_RAD * 60.0f ;  // degree per second


    this->_HOMING_SPEED[AXIS_BETA] = DEG_TO_RAD * 3;
}

void GobotHouse_CncSolutionConfig_2206::__InitStaticConfig(){
    // For actuator is stepper BYG-48
    // this->_link_a_length = 75.0;  
    // this->_LINK_B_LENGTH = 75.0; 

    // For actuator is servo 
    this->_LINK_A_LENGTH = 71.5;
    this->_LINK_B_LENGTH = 79.5;

    this->_home_via_inverse_kinematic = true;

    this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
    this->_HOMING_DIR_IS_TO_MAX[AXIS_BETA] = 1;

    // this->Homed_position_alpha_in_degree = 13.6;
    this->_HOMED_POSITION[AXIS_ALPHA] = 8.0f;
    this->_HOMED_POSITION[AXIS_BETA] = 128.0f;


}


// int32_t GobotHouse_CncSolutionConfig_2206::GetStepsPerUnit(char axis_name) {
//     return 100;
// }

// float GobotHouse_CncSolutionConfig_2206::GetHomingVelocity(char axis_name){
//     if (axis_name == AXIS_ALPHA)
//         // return DEG_TO_RAD * this->_homing_speed_alpha_degree_per_second  * this->HomingDir_IsToMax(AXIS_ALPHA);
//         return DEG_TO_RAD * this->HomingSpeed(AXIS_ALPHA)  * this->HomingDir_IsToMax(AXIS_ALPHA);
//     else if (axis_name == 'B'){
//         bool debug = false;
//         if(debug){
//             Serial.print("[Debug] GobotHouse_CncSolutionConfig_2206::GetHomingVelocity() _homing_speed_beta_degree_per_second = ");
//             // Serial.print(_homing_speed_beta_degree_per_second);
//             Serial.print(this->HomingSpeed(AXIS_BETA));
//             Serial.print("  HOMING_DIR_BETA_IS_TO_MAX = ");
//             Serial.println(this->HomingDir_IsToMax(AXIS_BETA));
//         }
//         // return DEG_TO_RAD * this->_homing_speed_beta_degree_per_second  * this->HomingDir_IsToMax(AXIS_BETA);
//         return DEG_TO_RAD * this->HomingSpeed(AXIS_BETA)  * this->HomingDir_IsToMax(AXIS_BETA);
//     }
//     else{
//         Serial.print("[Error] GobotHouse_CncSolutionConfig_2206::GetHomingVelocity() " );
//         Serial.println(axis_name);
//     }
//     return 0;
// }

void GobotHouse_CncSolutionConfig_2206::PrintOut(const char * title){
    Serial.print("\n\n===============================================");
    Serial.print("\n   GobotHouse_CncSolutionConfig_2206::PrintOut()");
    Serial.print("\n       Homed_position_alpha_in_degree= ");
    Serial.print(RAD_TO_DEG * this->Homed_position_alpha_in_rad);
    Serial.print("\n       Homed_position_beta_in_degree= "); 
    Serial.print(RAD_TO_DEG * this->Homed_position_beta_in_rad);

    // Serial.print("\n   MOTOR_MICRO_STEPS= ");
    // Serial.print(this->MOTOR_MICRO_STEPS);
    // Serial.print("\n       motor_steps_per_round=514* MOTOR_MICRO_STEPS= ");
    // Serial.print(this->motor_steps_per_round);
    // Serial.print("\n   GEAR_BOX_RATIO_ALPHA= ");
    // Serial.print(this->GEAR_BOX_RATIO_ALPHA);
    // Serial.print("\n   STEPS_PER_RAD_ALPHA= ");
    // Serial.print(this->STEPS_PER_RAD_ALPHA);
    // Serial.print("\n   MAX_STEPS_PER_SECOND_ALPHA= ");
    // Serial.print(this->MAX_STEPS_PER_SECOND_ALPHA);
    // Serial.print("\n   MAX_ACCELERATION_ALPHPA= ");
    // Serial.print(this->MAX_ACCELERATION_ALPHPA);

    // Serial.print("\n\n       STEPS_PER_RAD_BETA= ");
    // Serial.print(this->STEPS_PER_RAD_BETA);
    Serial.print("\n   GetHomingSpeed(BETA)= ");
    Serial.print(this->GetHomingVelocity(AXIS_ALPHA));
    Serial.print("\n   GetHomingSpeed(ALPHA)= ");
    Serial.print(this->GetHomingVelocity(AXIS_BETA));
    Serial.print("\n\n===============================================");
}
