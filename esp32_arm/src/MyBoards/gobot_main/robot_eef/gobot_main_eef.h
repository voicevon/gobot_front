#pragma once

#include "RobotEef/robot_eef_base.h"
#include "../board_ver1.2.h"
#include <ESP32Servo.h>

class RobotEef_GobotMain: public RobotEefBase{
    public:
        RobotEef_GobotMain();
        void Init();
        void Run(uint8_t eef_code) override;

    private:
        Servo __eefServo;

};