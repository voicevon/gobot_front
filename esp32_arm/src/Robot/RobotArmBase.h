#pragma once
// #include "ESP32Step/src/TeensyStep.h"
// #include "RobotArmBase.h"  //??
#include "Axis/SingleAxisBase.hpp"
#include "Robot/Gcode.h"
// #include <Arduino.h>

struct motor_position{
    int alpha;
    int beta;
};

/*
Currently is a 2D arm.
*/
template<class Actuator_T>
class RobotArmBase{
    public:
        // virtual void Home(unsigned char axis);
        virtual void HomeAllAxises();
        // void LinkStepper(Stepper* alpha, Stepper* beta);
        SingleAxisBase<Actuator_T>* axis_alpha;
        SingleAxisBase<Actuator_T>* axis_beta;
        void RunGcode(Gcode* gcode);
        bool IsBusy(){return __is_busy;};
        void OnFinishedGcode(void(*callback)(string message)) {__on_finished_gcode = callback;};

    protected:
        void MoveTo(int16_t x, int16_t y);
        virtual motor_position ik(float x, float y);

        // For High perfermance, Is this necessery?
        // Stepper* stepper_alpha;
        // Stepper* stepper_beta;
        // StepControl* steppers;

        void (* __on_finished_gcode)(string message);
    private:
        bool __is_busy = false;

};