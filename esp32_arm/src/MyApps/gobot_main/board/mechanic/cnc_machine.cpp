#include "cnc_machine.h"

// int32_t GobotMainMachine::GetStepsPerUnit(char axis_name) {
//     return 100;
// }

GobotMainMachine::GobotMainMachine(){
    this->_LINK_0_LENGTH = 191.0f / 2;
    this->_LINK_A_LENGTH = 278.1;  //285.18;    // 278.1
    this->_LINK_B_LENGTH = 392.1; //  384.51;    // 500-107.8

    this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
    this->_HOMING_SPEED[AXIS_ALPHA] = 1000;
    this->_HOMED_POSITION[AXIS_ALPHA] = DEG_TO_RAD * (180 - 4);

    this->_HOMING_DIR_IS_TO_MAX[AXIS_BETA] = -1;
    this->_HOMING_SPEED[AXIS_BETA] = 1000;
    this->_HOMED_POSITION[AXIS_BETA] = DEG_TO_RAD * 4;


}

void GobotMainMachine::PrintOut(const char* title){
    Logger::PrintTitle(title);
    Serial.print("Lenth of LINK_0, LINK_A, LINK_B = (");
    Serial.print(this->LINK_0);
    Serial.print(" , ");
    Serial.print(this->LINK_A);
    Serial.print(" , ");
    Serial.print(this->LINK_B);
    Serial.println(")");
    Serial.print("Homer.Alpha: ");
    Serial.print("  DirIsToMax= ");
    Serial.print(this->HomingDir_IsToMax(AXIS_ALPHA));
    Serial.print("  Homed_position_in_degree = ");
    Serial.println(RAD_TO_DEG * this->Homed_position_alpha_in_rad);
    Serial.print("Homer,Beta: " );
    Serial.print("  DirIsToMax= ");
    Serial.print(this->HomingDir_IsToMax(AXIS_BETA));
    Serial.print("  Homed_position_in_degree = ");
    Serial.print(RAD_TO_DEG * this->Homed_position_beta_in_rad);
    // Serial.print("\n STEPS_PER_RAD = ");
    // Serial.print(this->STEPS_PER_RAD);

    Serial.println(FCBC_RESET);

}