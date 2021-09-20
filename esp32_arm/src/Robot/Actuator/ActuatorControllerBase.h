#pragma once

// TODO: StepControl should be a child class.

/**
 * How one or a group of actuators move?
 * Who send out the command?
 *            Me !
 */

template<class Actuator_T>
class ActuatorControllerBase{
    public:
        ActuatorControllerBase(){};
        void LinkActuator(Actuator_T* actuator){this->_actuator= actuator;};
        virtual void Move(float relDistance);

    protected:
        Actuator_T _actuator;
};

