
#include "cnc_machine.h"

GobotHouseMachine_2206::GobotHouseMachine_2206(){
    this->Init('M');
}

void GobotHouseMachine_2206::__InitStaticConfig(){
    this->LINK_A = 75.0;
    this->LINK_B = 75.0;

    this->HOMING_DIR_ALPHA_IS_TO_MAX = 1;
    this->HOMING_DIR_BETA_IS_TO_MAX = 1;

    // this->Homed_position_alpha_in_degree = 13.6;
    this->Homed_position_alpha_in_degree = 8;
    this->Homed_position_beta_in_degree = 128.0;
}

void GobotHouseMachine_2206::Init(char solution){
    this->__InitStaticConfig();

    this->MAX_ACCELERATION_ALPHPA = 600 ;

    this->_homing_speed_alpha_degree_per_second =  99;  // degree per second
    this->Homing_acceleration_alpha = 200 ;

    this->_homing_speed_beta_degree_per_second = 3;    //degree per second.
    this->Homing_acceleration_beta = 0;
    this->PrintOut();
}


// int32_t GobotHouseMachine_2206::GetStepsPerUnit(char axis_name) {
//     return 100;
// }

float GobotHouseMachine_2206::GetHomingVelocity(char axis_name){
    if (axis_name == 'A')
        return DEG_TO_RAD * this->_homing_speed_alpha_degree_per_second  * this->HOMING_DIR_ALPHA_IS_TO_MAX;
    else if (axis_name == 'B'){
        bool debug = false;
        if(debug){
            Serial.print("[Debug] GobotHouseMachine_2206::GetHomingVelocity() _homing_speed_beta_degree_per_second = ");
            Serial.print(_homing_speed_beta_degree_per_second);
            Serial.print("  HOMING_DIR_BETA_IS_TO_MAX = ");
            Serial.println(HOMING_DIR_BETA_IS_TO_MAX);
        }
        return DEG_TO_RAD * this->_homing_speed_beta_degree_per_second  * this->HOMING_DIR_BETA_IS_TO_MAX;
    }
    else{
        Serial.print("[Error] GobotHouseMachine_2206::GetHomingVelocity() " );
        Serial.println(axis_name);
    }
    return 0;
}

void GobotHouseMachine_2206::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\n   GobotHouseMachine_2206::PrintOut()");
    Serial.print("\n       Homed_position_alpha_in_degree= ");
    Serial.print(this->Homed_position_alpha_in_degree);
    Serial.print("\n       Homed_position_beta_in_degree= "); 
    Serial.print(this->Homed_position_beta_in_degree);

    Serial.print("\n   MOTOR_MICRO_STEPS= ");
    // Serial.print(this->MOTOR_MICRO_STEPS);
    // Serial.print("\n       motor_steps_per_round=514* MOTOR_MICRO_STEPS= ");
    // Serial.print(this->motor_steps_per_round);
    // Serial.print("\n   GEAR_BOX_RATIO_ALPHA= ");
    // Serial.print(this->GEAR_BOX_RATIO_ALPHA);
    // Serial.print("\n   STEPS_PER_RAD_ALPHA= ");
    // Serial.print(this->STEPS_PER_RAD_ALPHA);
    Serial.print("\n   MAX_STEPS_PER_SECOND_ALPHA= ");
    Serial.print(this->MAX_STEPS_PER_SECOND_ALPHA);
    Serial.print("\n   MAX_ACCELERATION_ALPHPA= ");
    Serial.print(this->MAX_ACCELERATION_ALPHPA);

    // Serial.print("\n\n       STEPS_PER_RAD_BETA= ");
    // Serial.print(this->STEPS_PER_RAD_BETA);
    Serial.print("\n   GetHomingSpeed('A')= ");
    Serial.print(this->GetHomingVelocity('A'));
    Serial.print("\n   GetHomingSpeed('B')= ");
    Serial.print(this->GetHomingVelocity('B'));
    Serial.print("\n\n===============================================");
}
