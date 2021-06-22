#pragma once

#include "actions.h"
#include <Arduino.h>
#include <ESP32Servo.h>
// #include "esp32-hal-ledc.h"


class House{
    public:
        House();
        void Init(void);
        void SpinOnce(HouseAction action);
        void DrawStone(int house_id);
        void MoveStoneToTarget(int start_point);
    protected:
    private:
        Servo __LeftServo;
        Servo __RightServo;
        void EnableExpandPin(int phisical_pin_id);


};