
#include "actuator_alpha_beta.h"

GobotMain_ActuatorAlphaBetaMechanic::GobotMain_ActuatorAlphaBetaMechanic(){
    this->_micro_steps_on_stepper_driver = 16;
    this->_motor_gear_teeth_count = 10;
    this->_slave_pulley_teeth_count = 90;
    this->_motor_step_angle_in_degree = 0.7003891050583658;
}




#include "cnc_machine.h"

GoboMainMachine::GoboMainMachine(){
    this->Init('M');
}

void GoboMainMachine::__InitStaticConfig(){
    // For actuator is stepper BYG-48
    // this->_link_a_length = 75.0;  
    // this->_LINK_B_LENGTH = 75.0; 

    // For actuator is servo 
    this->_LINK_A_LENGTH = 71.5;
    this->_LINK_B_LENGTH = 79.5;

    // this->HOMING_DIR_ALPHA_IS_TO_MAX = 1;
    this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
    // this->HOMING_DIR_BETA_IS_TO_MAX = 1;
    this->_HOMING_DIR_IS_TO_MAX[AXIS_BETA] = 1;
    // this->Homed_position_alpha_in_degree = 13.6;
    // this->_homed_position_alpha_in_degree = 8;
    // this->_homed_position_beta_in_degree = 128.0;
    this->_HOMED_POSITION[AXIS_ALPHA] = 8.0f;
    this->_HOMED_POSITION[AXIS_BETA] = 128.0f;
}

void GoboMainMachine::Init(char solution){
    this->__InitStaticConfig();

    // this->MAX_ACCELERATION_ALPHPA = 600 ;

    // this->_homing_speed_alpha_degree_per_second =  60;  // degree per second
    this->_HOMING_SPEED[AXIS_ALPHA] = 60;    // TODO : to rad
    // this->Homing_acceleration_alpha = 200 ;

    // this->_homing_speed_beta_degree_per_second = 3;    //degree per second.
    this->_HOMING_SPEED[AXIS_BETA] = 3;
    // this->Homing_acceleration_beta = 0;
    this->PrintOut();
}


// int32_t GoboMainMachine::GetStepsPerUnit(char axis_name) {
//     return 100;
// }

float GoboMainMachine::GetHomingVelocity(char axis_name){
    if (axis_name == 'A')
        // return DEG_TO_RAD * this->_homing_speed_alpha_degree_per_second  * this->HOMING_DIR_ALPHA_IS_TO_MAX;
        // return DEG_TO_RAD * this->_homing_speed_alpha_degree_per_second  * this->HomingDir_IsToMax(AXIS_ALPHA);
        return this->HomingSpeed(AXIS_ALPHA)  * this->HomingDir_IsToMax(AXIS_ALPHA);
    else if (axis_name == 'B'){
        bool debug = false;
        if(debug){
            Serial.print("[Debug] GoboMainMachine::GetHomingVelocity() _homing_speed_beta_degree_per_second = ");
            // Serial.print(_homing_speed_beta_degree_per_second);
            Serial.print(this->HomingSpeed(AXIS_BETA));
            Serial.print("  HOMING_DIR_BETA_IS_TO_MAX = ");
            Serial.println(this->HomingDir_IsToMax(AXIS_BETA));
        }
        // return DEG_TO_RAD * this->_homing_speed_beta_degree_per_second  * this->HOMING_DIR_BETA_IS_TO_MAX;
        // return DEG_TO_RAD * this->_homing_speed_beta_degree_per_second  * this->HomingDir_IsToMax(AXIS_BETA);
        return this->HomingSpeed(AXIS_BETA)  * this->HomingDir_IsToMax(AXIS_BETA);
    }
    else{
        Serial.print("[Error] GoboMainMachine::GetHomingVelocity() " );
        Serial.println(axis_name);
    }
    return 0;
}

void GoboMainMachine::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\n   GoboMainMachine::PrintOut()");
    Serial.print("\n       Homed_position_alpha_in_degree= ");
    Serial.print(RAD_TO_DEG * this->Homed_position_alpha_in_rad);
    Serial.print("\n       Homed_position_beta_in_degree= "); 
    Serial.print(RAD_TO_DEG * this->Homed_position_beta_in_rad);

    Serial.print("\n   MOTOR_MICRO_STEPS= ");
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
    Serial.print("\n   GetHomingSpeed('A')= ");
    Serial.print(this->GetHomingVelocity('A'));
    Serial.print("\n   GetHomingSpeed('B')= ");
    Serial.print(this->GetHomingVelocity('B'));
    Serial.print("\n\n===============================================");
}
