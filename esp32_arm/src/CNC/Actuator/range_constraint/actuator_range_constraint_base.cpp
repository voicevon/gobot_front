#include "actuator_range_constraint_base.h"
#include "von/utility/logger.h"


float ActuatorRangeConstraintBase::_ConvertTo_ActuatorRange(float cnc_position_in_rad){
    // int range_min_in_degree = -315;
    // int range_max_in_degree = 20;
    float actuator_position = cnc_position_in_rad;
    if(actuator_position >  this->_actuator_range_max){
        actuator_position = cnc_position_in_rad - TWO_PI;
    }
    return actuator_position;
}

float ActuatorRangeConstraintBase::_ConvertTo_CncRange(float actuator_position){
    float cnc_position = actuator_position;
    if(cnc_position <  this->_cnc_range_min){
        cnc_position = actuator_position + TWO_PI;
    }
    return cnc_position;
}