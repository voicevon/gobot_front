#include "cnc_machine.h"

int32_t GobotMainMachine::GetStepsPerUnit(char axis_name) {
    return 100;
}

void GobotMainMachine::Init(char solution){
    this->LINK_0 = 191.0f / 2;
    this->LINK_A = 278.1;  //285.18;    // 278.1
    this->LINK_B = 392.1; //  384.51;    // 500-107.8

    this->STEPPER_DRIVER_MICRO_STEPS = 16;
    this->MOTOR_STEP_ANGLE = 1.8;
    this->motor_steps_per_shaft_round = 360.0 / this->MOTOR_STEP_ANGLE * this->STEPPER_DRIVER_MICRO_STEPS;
    this->GEAR_BOX_RATIO = 27.0f / 1.0f;   // Big gear = 90 teeth, small gear = 20 teeth
    
    this->STEPS_PER_RAD = this->motor_steps_per_shaft_round * this->GEAR_BOX_RATIO / TWO_PI;

    this->MAX_STEPS_PER_SECOND_ALPHA_BETA = 9000;
    this->MAX_ACCELERATION_ALPHA_BETA = 3000;

    this->Homing_acceleration_alpha_beta = 2000;
    this->Homing_speed_alpha_beta = 1000;
    this->Homed_position_alpha_in_degree = 180 - 4;
    this->Homed_position_beta_in_degree = 4;


}

void GobotMainMachine::PrintOut(const char* title){
    Serial.print("\n=========================================================");
    Serial.print("\n      GobotMainMachine::Printout()    "); 
    Serial.print(title);

    Serial.print("\n Lenth of LINK_0, LINK_A, LINK_B = (");
    Serial.print(this->LINK_0);
    Serial.print(" , ");
    Serial.print(this->LINK_A);
    Serial.print(" , ");
    Serial.print(this->LINK_B);
    Serial.print(")");
    Serial.print("\n GEAR_BOX_RATIO = ");
    Serial.print(this->GEAR_BOX_RATIO);
    Serial.print("\n STEPPER_DRIVER_MICRO_STEPS = ");
    Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);
    
    Serial.print("\n\n Homed_position_alpha_in_degree = ");
    Serial.print(this->Homed_position_alpha_in_degree);
    Serial.print("\n Homed_position_beta_in_degree = ");
    Serial.print(this->Homed_position_beta_in_degree);
    Serial.print("\n STEPS_PER_RAD = ");
    Serial.print(this->STEPS_PER_RAD);

    Serial.print("\n\n\n");

}