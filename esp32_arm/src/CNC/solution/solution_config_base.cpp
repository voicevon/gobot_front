#include "solution_config_base.h"

float CncSolutionConfigBase::GetLongOffsetToGoHome(EnumAxis axis){
    return this->_LONG_DISTANCE_TO_HOME[axis] * this->_HOMING_DIR_IS_TO_MAX[axis];
}

