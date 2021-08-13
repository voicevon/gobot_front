#pragma once

#include "actions.h"
#include <Arduino.h>
#include "RobotArm.h"

#define COIL_COUNT 53
#define CHIPS_COUNT  4


class House:RobotArm{
    public:
        static House& getInstance()
        {
            static House instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void SpinOnce(void);
        void Setup(RobotAction* pAction);
        void Home(uint8_t axis) override;
        void DrawStone(uint8_t house_id);
        void MoveStoneToTarget(uint8_t start_point);

    protected:
    private:
        House();
        RobotAction* __house_action;
        
        motor_position ik(int x, int y) override;
        void __HomeSpin(Stepper* homing_stepper, uint8_t home_pin);

};