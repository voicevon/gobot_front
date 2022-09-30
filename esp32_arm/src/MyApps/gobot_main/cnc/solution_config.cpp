// #include "solution_config.h"
// #include "actuator_alpha_beta.h"

// GobotMain_CncSolutionConfig::GobotMain_CncSolutionConfig(){

//     this->_LINK_0_LENGTH = 191.0f / 2;
//     this->_LINK_A_LENGTH = 278.1;  //285.18;    // 278.1
//     this->_LINK_B_LENGTH = 392.1; //  384.51;    // 500-107.8
//     // GobotMain_Actuator_MechanicConfig config;
//     this->_ACCELERATION_FOR_G1[AXIS_ALPHA] = DEG_TO_RAD * 60;
//     this->_ACCELERATION_FOR_G1[AXIS_BETA] = DEG_TO_RAD * 60;

//     this->_home_via_inverse_kinematic = true;

    
//     // this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
//     // this->_HOMED_POSITION[AXIS_ALPHA] = DEG_TO_RAD * (180 - 4);
//     // this->_LONG_DISTANCE_TO_HOME[AXIS_ALPHA] = 9.9f;
//     // this->_HOMING_SPEED[AXIS_ALPHA] = DEG_TO_RAD * 10;
//     // this->_HOMING_ACCELERATION[AXIS_ALPHA] = DEG_TO_RAD * 30;

//     // this->_HOMING_DIR_IS_TO_MAX[AXIS_BETA] = -1;
//     // this->_HOMED_POSITION[AXIS_BETA] = DEG_TO_RAD * 4;
//     // this->_LONG_DISTANCE_TO_HOME[AXIS_BETA] = 9.9f;
//     // this->_HOMING_SPEED[AXIS_BETA] = DEG_TO_RAD * 10;
//     // this->_HOMING_ACCELERATION[AXIS_BETA] = DEG_TO_RAD * 30;

// }

// void GobotMain_CncSolutionConfig::PrintOut(const char* title){
//     Logger::Info(title);
//     Serial.print("Lenth of (LINK_0, LINK_A, LINK_B) = (");
//     Serial.print(this->LINK_0);
//     Serial.print(" , ");
//     Serial.print(this->LINK_A);
//     Serial.print(" , ");
//     Serial.print(this->LINK_B);
//     Serial.println(")");
//     Serial.print("Is Inverse kinematic homing= ");
//     Serial.println(this->IsInverseKinematicHoimg);
//     Logger::Print("Alpha","");
//     Logger::Print("\t_ACCELERATION", this->_ACCELERATION_FOR_G1[AXIS_ALPHA]);
//     Serial.println("Home ");
//     // Logger::Print("\tDirIsToMax", this->HomingDir_IsToMax(AXIS_ALPHA));
//     // Logger::Print("\tDistanceToGoHome", this->GetLongOffsetToGoHome(AXIS_ALPHA));
//     // Logger::Print("\tHomed_position_in_degree", RAD_TO_DEG * this->Homed_position_alpha_in_rad);
//     // Logger::Print("\tSpeed", RAD_TO_DEG * this->HomingSpeed(AXIS_ALPHA) );
//     // Logger::Print("\tAcceleration", RAD_TO_DEG * this->HomingAcceleration(AXIS_ALPHA));

//     Serial.println("Beta:" );
//     Logger::Print("\t_ACCELERATION", this->_ACCELERATION_FOR_G1[AXIS_BETA]);
//     Serial.println("Home ");
//     // Logger::Print("\tDirIsToMax", this->HomingDir_IsToMax(AXIS_BETA));
//     // Logger::Print("\tDistanceToGoHome", this->GetLongOffsetToGoHome(AXIS_BETA));
//     // Logger::Print("\tHomed_position_in_degree", RAD_TO_DEG * this->Homed_position_beta_in_rad);
//     // Logger::Print("\tSpeed",  RAD_TO_DEG * this->HomingSpeed(AXIS_BETA));
//     // Logger::Print("\tAcceleration", RAD_TO_DEG * this->HomingAcceleration(AXIS_BETA));
//     Serial.println(FCBC_RESET);

// }