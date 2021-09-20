#pragma once
#include "Axis/SingleAxisBase.hpp"
#include "Robot/Gcode.h"



/*
Currently is a 2D arm.
*/
template<class Actuator_T, class ActuatorController_T,class CommuDevice_T>
class RobotArmBase{
    public:
        void SpinOnce();
        virtual void HomeAllAxises();
        SingleAxisBase<Actuator_T> axis_alpha;
        SingleAxisBase<Actuator_T> axis_beta;
        void LinkActuatorController(ActuatorController_T* controller){this->actuatorController = controller;};
        void LinkCommuDevice(CommuDevice_T* commuDevice){this->commuDevice=commuDevice;};
        void RunGcode(Gcode* gcode);
        bool IsBusy(){return __is_busy;};
        // void OnOutputMessage_set_callback(void(*callback)(string message)) {__output_message = callback;};

    protected:
        void MoveTo(int16_t x, int16_t y);
        virtual ik_position ik(float x, float y);

        // For High perfermance, Is this necessery?
        // Stepper* stepper_alpha;
        // Stepper* stepper_beta;
        ActuatorController_T* actuatorController;
        CommuDevice_T* commuDevice;

        // void (* __output_message)(string message);
    private:
        bool __is_busy = false;

};