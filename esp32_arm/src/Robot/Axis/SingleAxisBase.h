#pragma once

#include <Arduino.h>
#include "../HomeTriger.h"
#include "ESP32Step/src/Stepper.h"

// http://www.gammon.com.au/forum/?id=12983     static instances code for attachInterupt()
// https://www.fluentcpp.com/2018/01/09/strong-templates/


/**
 *  SingleAxis can NOT move, because it has no ActuatorController.
 *  Saying: An axis can NOT do home, Home() should be in top level, it is an Robot().
*/
template <class Actuator_T>
class SingleAxisBase{
    public:
        // SingleAxisBase(const char axis_name);
        SingleAxisBase(){};
        char Name;
        void SpinOnce();

        bool IsBusy(){return __is_busy;};
        void LinkAcuator(Actuator_T* actuator);
        // virtual void Home(){};   //??
        void LinkHomeTriger(HomeTriger* homeTriger);   //Homer = axis(actuaotr+driver) + controller + trigger 
        void Init_scaler(float final_distance_per_encoder_interval);
        Actuator_T* _actuator;

    protected:
        HomeTriger* homeTriger;
        // void SetTargetAbs(int targetPosition);
        // virtual void Move(float distanceRel);
        // virtual void MoveAsync();

        char _Axis_Name;
        float home_position;
        float final_distance_per_encoder_interval;
        bool __is_busy = false;
        bool __is_homing = false;

};
