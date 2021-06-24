#pragma once

#include "actions.h"
#include "mcp23018.h"
#include <Arduino.h>
#include <ESP32Servo.h>
// #include "esp32-hal-ledc.h"


class House{
    public:
        static House& getInstance()
        {
            static House instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void SpinOnce(HouseAction action);
        void DrawStone(int house_id);
        void MoveStoneToTarget(int start_point);
    protected:
    private:
        House();
        // House(House const& copy);            // Not Implemented
        // House& operator=(House const& copy); // Not Implemented

        Servo __LeftServo;
        Servo __RightServo;
        Mcp23018* __Mcp23018;
        // void __EnableSingleCoil(int phisical_pin_id, bool enable_it);
        


};