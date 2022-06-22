#include "actuator_base.h"

void ActuatorBase::LinkRangeConstraint(ActuatorRangeConstraintBase* range_constraint){
    this->_range_constraint = range_constraint;
    this->_is_range_constraint = true;
}
