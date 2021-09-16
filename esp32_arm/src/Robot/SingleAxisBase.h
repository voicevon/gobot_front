#pragma once

#include <Arduino.h>
#include "MotorBase.h"
// #include "FunctionalInterrupt.h"
// #include "SimpleFOC/SimpleFOC.h"
// #include "MyBoards/board_pipe_bot_corner.h"

// http://www.gammon.com.au/forum/?id=12983     static instances code for attachInterupt()


// https://www.fluentcpp.com/2018/01/09/strong-templates/

template <class Actuator_T, class PositionSensor_T>
class SingleAxisBase{
    public:
        uint8_t id;
        Actuator_T* actuator;
        PositionSensor_T* sensor;
        void LinkAcuator(Actuator_T act){};
        void LinkSensor(PositionSensor_T sensor){};
        // Motor* motor;
        // void test(){motor->Move2};};

        void SetTargetAbs(int targetPosition);
        virtual void Move(float distanceRel){};
        virtual void MoveAsync(){};
        virtual void Home(){};


        void Init_scaler(float final_distance_per_encoder_interval);
        void Test();

    protected:
        uint8_t __pinHomeSensor;
        float home_position;
        float final_distance_per_encoder_interval;
};
