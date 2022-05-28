#pragma once

// TODO: StepControl should be a child class.

#include "CNC/Actuator/ActuatorBase.h"

/**
 * How one or a group of actuators move?
 * Who send out the command?
 *            Me !
 */
// template<class Actuator_T>
class ActuatorControllerBase{
    public:
        ActuatorControllerBase(){};
        // void LinkActuator(ActuatorBase* actuator){this->_actuator= actuator;};
        // virtual void LinkDriver();  //??
        virtual void Move(float relDistance){};
        virtual void MoveTo(float absPosition){};
        virtual void MoveForward(float speed, bool reverseDir){};  // For DC Motor.

    protected:
        ActuatorBase* _actuator;
};

