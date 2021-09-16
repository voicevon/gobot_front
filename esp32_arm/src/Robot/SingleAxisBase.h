#pragma once

#include <Arduino.h>
#include "MotorBase.h"
// #include "FunctionalInterrupt.h"
// #include "SimpleFOC/SimpleFOC.h"
// #include "MyBoards/board_pipe_bot_corner.h"

// http://www.gammon.com.au/forum/?id=12983     static instances code for attachInterupt()


// https://www.fluentcpp.com/2018/01/09/strong-templates/

// template <MotorBase Motor>
class SingleAxisBase{
    public:
        uint8_t id;
        // Motor* motor;
        void Test();
        // void test(){motor->Move2};};
        void Init_scaler(float final_distance_per_encoder_interval);
        void SetTargetAbs(int targetPosition);
        virtual void Move(float distanceRel);
        virtual void MoveAsync();
        virtual void Home();

    protected:
        uint8_t __pinHomeSensor;
        float home_position;
        float final_distance_per_encoder_interval;
};
