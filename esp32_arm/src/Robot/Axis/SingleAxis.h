#pragma once

#include <Arduino.h>
// #include "../HomeTriger.h"
#include "ESP32Step/src/Stepper.h"
#include "SimpleFOC/sensors/Encoder.h"
#include "Robot/Actuator/ActuatorBase.h"

// http://www.gammon.com.au/forum/?id=12983     static instances code for attachInterupt()
// https://www.fluentcpp.com/2018/01/09/strong-templates/


/**
 *  SingleAxis can NOT move, because it has no ActuatorController.
 *  Saying: An axis can NOT do home, Home() should be in top level, it is an Robot().
*/

class SingleAxis{
    public:
        // SingleAxisBase(const char axis_name);
        SingleAxis(){};
        char Name;
        void SpinOnce();

        bool IsBusy(){return __is_busy;};
        void LinkAcuator(ActuatorBase* actuator){};
        // void LinkSensor(Sensor* sensor){};
        // virtual void Home(){};   //??
        // void LinkHomeTriger(HomeTriger* homeTriger);   //Homer = axis(actuaotr+driver) + controller + trigger 
        void Init_scaler(float final_distance_per_encoder_interval);
        ActuatorBase* _actuator;
        // Sensor* sensor;
        char _Axis_Name;
        void SetCurrentPosition(float position);
    protected:
        float from_actuator_position(){return this->_actuator->GetCurrentPos();};
        float to_actuator_position(float position){return position;};
        
        // HomeTriger* homeTriger;
        // void SetTargetAbs(int targetPosition);
        // virtual void Move(float distanceRel);
        // virtual void MoveAsync();

        // float home_position;
        float final_distance_per_encoder_interval;
        bool __is_busy = false;
        // bool __is_homing = false;

};
