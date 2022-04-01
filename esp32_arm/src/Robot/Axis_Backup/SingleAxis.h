#pragma once

#include <Arduino.h>
// #include "../HomeTriger.h"
#include "ESP32Step/src/Stepper.h"
// #include "SimpleFOC/sensors/Encoder.h"
#include <SimpleFOC.h>
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
        void SpinOnce();
        bool IsBusy(){return __is_busy;};
        void LinkAcuator(ActuatorBase* actuator){this->_actuator=actuator;};
        void Init_scaler(float final_distance_per_encoder_interval);
        ActuatorBase* _actuator;
        char Name;
        void SetCurrentPosition(float position);

    protected:
        float from_actuator_position(){return this->_actuator->GetCurrentPos();};
        float to_actuator_position(float position){return position;};
        
        float final_distance_per_encoder_interval;
        bool __is_busy = false;

};
