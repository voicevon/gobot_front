// #include "cnc_machine.h"
#include "cnc_solution_config.h"
#include "HardwareSerial.h"

// void BoxCarrierMachine::Init(char solution_id){
//     this->__Init_Common();
//     // if (solution_id == 'F')
//         this->__Init_Fast();
// }
void BoxCarrierMachine::__Init_Common(){
    this->MOTOR_DEGREE_PER_STEP = 1.8;
    this->MOTOR_GEARBOX_RATIO_ALPHA_BETA = 27;
    this->MOTOR_PULLEY_PITCH = 2;
    this->MOTOR_PULLEY_TEETH_COUNT = 20;
    this->STEPPER_DRIVER_MICRO_STEPS = 16;
}

void BoxCarrierMachine::__Init_Fast(){

        this->steps_per_motor_round = 360.0f / MOTOR_DEGREE_PER_STEP * STEPPER_DRIVER_MICRO_STEPS * MOTOR_GEARBOX_RATIO_ALPHA_BETA;

        float steps_per_origin_mm = this->steps_per_motor_round / MOTOR_PULLEY_TEETH_COUNT / MOTOR_PULLEY_PITCH; 
        // this->steps_per_mm_for_z = steps_per_origin_mm / 1.414;
        // this->steps_per_mm_for_y = steps_per_origin_mm / 1.414;
        this->steps_per_mm_for_z = steps_per_origin_mm ;
        this->steps_per_mm_for_y = steps_per_origin_mm ;


        this->Homing_speed_alpha_beta = 6000;
        this->Homing_acceleration_alpha_beta = 5000;
        this->Homed_position_y = 0;
        this->Homed_position_z = 0;

        this->max_acceleration_alpha_beta = 6000;
        this->max_speed_alpha_beta = 20000;

}

void BoxCarrierMachine::PrintOut(const char* title){
    Serial.print("\n\n===============================================");
    Serial.print("\n   BoxMoverHardwareConfig::PrintOut()");
    Serial.print("\n   STEPPER_DRIVER_MICRO_STEPS = ");
    Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);
    Serial.print("\n   MOTOR_GEARBOX_RATIO_ALPHA_BETA = ");
    Serial.print(this->MOTOR_GEARBOX_RATIO_ALPHA_BETA);
    Serial.print("\n   steps_per_motor_round = ");
    Serial.print(this->steps_per_motor_round);

    Serial.print("\n      steps_per_mm_for_z = ");
    Serial.print(this->steps_per_mm_for_z);
    Serial.print("\n      steps_per_rad_for_y = ");
    Serial.print(this->steps_per_mm_for_y);
    Serial.print("\n          Home_position (z,y) = (");
    Serial.print(this->Homed_position_z);
    Serial.print(" , ");
    Serial.print(this->Homed_position_y);
    Serial.print(" )");

    Serial.print("\n          Homing_speed_alpha_beta = ");
    Serial.print(this->Homing_speed_alpha_beta);
    Serial.print("\n          Homing_acceleration_alpha_beta = ");
    Serial.print(this->Homing_acceleration_alpha_beta);

    Serial.print("\n\n    max_speed_alpha_beta = ");
    Serial.print(max_acceleration_alpha_beta);
    Serial.print("\n      max_acceleration_alpha_beta = ");
    Serial.print(max_acceleration_alpha_beta);


    Serial.print("\n----------------------------------------------\n\n");
}


// int32_t BoxCarrierMachine::GetStepsPerUnit(char axis_name) {
//     return 123;
// }
