#include "solution_config.h"



Vsc_CncSolutionConfig::Vsc_CncSolutionConfig(){
        this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
        this->_HOMING_SPEED[AXIS_ALPHA] = 100.0f;
        this->_LONG_DISTANCE_TO_HOME[AXIS_ALPHA] = PI;   // half PI, with 4 position triggers.
}

void Vsc_CncSolutionConfig::PrintOut(const char* title){
    Logger::Debug("Vsc_CncSolutionConfig::PrintOut()");
    Logger::Print("Home_position (alpha)", this->HomedPosition(AXIS_ALPHA));
    Logger::Print("Homing_speed_alpha",this->HomingSpeed(AXIS_ALPHA));

    // Serial.print("\n\n   max_speed_alpha = ");
    // Serial.print(max_acceleration_alpha);

}

