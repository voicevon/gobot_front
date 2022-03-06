#include "gobot_chessboard_hw_config.h"
#include "HardwareSerial.h"
#include "Arduino.h"  // for TWO_PI, PI only

GobotChessboardHardwareConfig::GobotChessboardHardwareConfig(){
    this->Init();
    this->PrintOut();
}

void GobotChessboardHardwareConfig::Init(){
    this->STEPPER_DRIVER_MICRO_STEPS = 16;
    this->GEAR_BOX_RATIO = 5.8;
    this->MOTOR_STEPS_PER_ROUND = 360.0 / this->MOTOR_STEP_ANGLE * this->STEPPER_DRIVER_MICRO_STEPS;
    this->STEPS_PER_RAD = this->MOTOR_STEPS_PER_ROUND * this->GEAR_BOX_RATIO / TWO_PI;

}

void GobotChessboardHardwareConfig::PrintOut(){
    Serial.print("\n=========================================================");
    Serial.print("\n      GobotChessboardHardwareConfig::Printout()\n");
    Serial.print("\n=========================================================");

    Serial.print("\n Lenth of LINK_0, LINK_A, LINK_B = (");
    Serial.print(this->LINK_0);
    Serial.print(" , ");
    Serial.print(this->LINK_A);
    Serial.print(" , ");
    Serial.print(this->LINK_B);
    Serial.print(")");
    Serial.print("\n GEAR_BOX_RATIO = ");
    Serial.print(this->GEAR_BOX_RATIO);

    Serial.print("\n\n Homed_position_alpha_in_degree = ");
    Serial.print(this->Homed_position_alpha_in_degree);
    Serial.print("\n Homed_position_beta_in_degree = ");
    Serial.print(this->Homed_position_beta_in_degree);
    Serial.print("\n STEPS_PER_RAD = ");
    Serial.print(this->STEPS_PER_RAD);

    Serial.print("\n STEPPER_DRIVER_MICRO_STEPS = ");
    Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);

    Serial.print("\n\n\n");

}