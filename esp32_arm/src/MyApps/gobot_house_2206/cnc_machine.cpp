
// #include "cnc_machine.h"


// void GobotHouseMachine_2206::Init(char solution){
//         // [when microstep==1] The value 514.0 is measured/Calibrated from running 10 rounds, Not calculated from anywhere.
//         // This value should be calculated from motor's datasheet.  But can not get datasheet for 28-BGY motor.
//         // this->motor_steps_per_round = 514 ;

//         // this->STEPS_PER_RAD_ALPHA = this->motor_steps_per_round  / TWO_PI;  
//         // this->STEPS_PER_RAD_BETA =  this->motor_steps_per_round * this->GEAR_BOX_RATIO_BETA / TWO_PI; 
//         this->MAX_ACCELERATION_ALPHPA = 600 ;
//         this->MAX_STEPS_PER_SECOND_ALPHA = 4000 ;    // tested 1200          
//         // this->MAX_ACCELERATION_BETA = 200 * this->MOTOR_MICRO_STEPS;
//         // this->MAX_STEPS_PER_SECOND_BETA =  500 * this->MOTOR_MICRO_STEPS; 

//         this->HOMING_DIR_ALPHA_IS_TO_MAX = 1;
//         this->Homing_speed_alpha_degree_per_second =  99;  // degree per second
//         this->Homing_acceleration_alpha = 200 ;

//         this->HOMING_DIR_BETA_IS_TO_MAX = 1;
//         this->Homing_speed_beta_degree_per_second = 3;    //degree per second.
//         this->Homing_acceleration_beta = 0;


//         this->Homed_position_alpha_in_degree = 13.6;
//         this->Homed_position_beta_in_degree = 138.0;
//         // this->Homed_position_beta_in_degree = 148.0;

//         this->PrintOut();
// }


// // int32_t GobotHouseMachine_2206::GetStepsPerUnit(char axis_name) {
// //     return 100;
// // }

// float GobotHouseMachine_2206::GetHomingVelocity(char axis_name){
//     if (axis_name == 'A')
//         return DEG_TO_RAD * this->Homing_speed_alpha_degree_per_second  * this->HOMING_DIR_ALPHA_IS_TO_MAX;
//     else if (axis_name == 'B')
//         return DEG_TO_RAD * this->Homing_speed_beta_degree_per_second  * this->HOMING_DIR_BETA_IS_TO_MAX;
// }

// void GobotHouseMachine_2206::PrintOut(){
//     Serial.print("\n\n===============================================");
//     Serial.print("\n   GobotHouseMachine_2206::PrintOut()");
//     Serial.print("\n       Homed_position_alpha_in_degree= ");
//     Serial.print(this->Homed_position_alpha_in_degree);
//     Serial.print("\n       Homed_position_beta_in_degree= "); 
//     Serial.print(this->Homed_position_beta_in_degree);

//     Serial.print("\n   MOTOR_MICRO_STEPS= ");
//     // Serial.print(this->MOTOR_MICRO_STEPS);
//     // Serial.print("\n       motor_steps_per_round=514* MOTOR_MICRO_STEPS= ");
//     // Serial.print(this->motor_steps_per_round);
//     // Serial.print("\n   GEAR_BOX_RATIO_ALPHA= ");
//     // Serial.print(this->GEAR_BOX_RATIO_ALPHA);
//     // Serial.print("\n   STEPS_PER_RAD_ALPHA= ");
//     // Serial.print(this->STEPS_PER_RAD_ALPHA);
//     Serial.print("\n   MAX_STEPS_PER_SECOND_ALPHA= ");
//     Serial.print(this->MAX_STEPS_PER_SECOND_ALPHA);
//     Serial.print("\n   MAX_ACCELERATION_ALPHPA= ");
//     Serial.print(this->MAX_ACCELERATION_ALPHPA);

//     // Serial.print("\n\n       STEPS_PER_RAD_BETA= ");
//     // Serial.print(this->STEPS_PER_RAD_BETA);
//     Serial.print("\n   GetHomingSpeed('A')= ");
//     Serial.print(this->GetHomingVelocity('A'));
//     Serial.print("\n   GetHomingSpeed('B')= ");
//     Serial.print(this->GetHomingVelocity('B'));
//     Serial.print("\n\n===============================================");
// }
