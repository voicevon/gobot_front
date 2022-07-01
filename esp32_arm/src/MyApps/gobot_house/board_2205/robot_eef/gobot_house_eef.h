#pragma once

// #include "RobotEef/robot_eef_base.h"
#include "Robot/eef/robot_eef_base.h"
#include "../board_pins/board_pins_ver3.11.h"
#include <ESP32Servo.h>


class RobotEef_GobotHouse: public RobotEefBase{
    public:
        RobotEef_GobotHouse();
        void Init();
        void Run(uint8_t eef_code) override;

    private:
        Servo __eefServo;

};