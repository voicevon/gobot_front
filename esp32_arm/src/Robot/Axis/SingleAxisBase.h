#pragma once

#include <Arduino.h>
#include "../HomeTriger.h"
#include "ESP32Step/src/Stepper.h"

// http://www.gammon.com.au/forum/?id=12983     static instances code for attachInterupt()
// https://www.fluentcpp.com/2018/01/09/strong-templates/


//  An actuator knows its own position, Include an encoder or other position sensor
template <class Actuator_T>
class SingleAxisBase{
    public:
        // SingleAxisBase(const char axis_name);
        SingleAxisBase(){};
        char Name;
        void SpinOnce();

        virtual void Home(){};
        bool IsBusy(){return __is_busy;};
        void LinkAcuator(Actuator_T* actuator);
        void LinkHomeTriger(HomeTriger* homeTriger);
        void Init_scaler(float final_distance_per_encoder_interval);
        Actuator_T* _actuator;

    protected:
        HomeTriger* homeTriger;
        void SetTargetAbs(int targetPosition);
        virtual void Move(float distanceRel);
        virtual void MoveAsync();

        char _Axis_Name;
        float home_position;
        float final_distance_per_encoder_interval;
        bool __is_busy = false;
        bool __is_homing = false;

};
