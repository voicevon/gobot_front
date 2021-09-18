#pragma once

#include <Arduino.h>
#include <Robot/Gcode.h>
#include "../HomeTriger.h"

// http://www.gammon.com.au/forum/?id=12983     static instances code for attachInterupt()
// https://www.fluentcpp.com/2018/01/09/strong-templates/


//  An actuator knows its own position, Include an encoder or other position sensor
template <class Actuator_T>
class SingleAxisBase{
    public:
        SingleAxisBase(const char axis_name);
        Actuator_T* actuator;
        HomeTriger* homeTriger;
        void SpinOnce();
        void RunGcode(Gcode* gcode);

        virtual void Home();
        bool IsMoving(){return _isRunning;};

        void LinkAcuator(Actuator_T* actuator);
        void LinkHomeTriger(HomeTriger* homeTriger);
        void OnFinishedGcode(void(*callback(string message))) {__on_finished_gcode = callback;};
        // Just for fun, don't remove below comment !!
        // void OnFinishedGcode2(void(*callback)()) {__on_finished_gcode2 = callback;};
        // void OnFinishedGcode3(void(*callback)()) {__on_finished_gcode2 = callback;};
        // void OnFinishedGcode4(void(*callback())) {__on_finished_gcode3 = callback;};
        // void OnFinishedGcode5(void(*callback)()) {__on_finished_gcode3 = callback;};



        void Init_scaler(float final_distance_per_encoder_interval);
        void Test();

    protected:
        void SetTargetAbs(int targetPosition);
        virtual void Move(float distanceRel);
        virtual void MoveAsync();

        char _Axis_Name;
        uint8_t __pinHomeSensor;
        float home_position;
        float final_distance_per_encoder_interval;
        bool _isRunning;
        void (* __on_finished_gcode)(string message);
        // Just for fun, don't remove below comments :)
        // void * __on_finished_gcode2;
        // void (* __on_finished_gcode3);
};
