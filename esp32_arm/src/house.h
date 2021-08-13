#pragma once

#include "actions.h"
// #include "mcp23018.h"
#include <Arduino.h>
#include "RobotArm.h"
// #include <AccelStepper.h>
// #include <ESP32Servo.h>
// #include "esp32-hal-ledc.h"

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
        void Home(uint8_t axis);
        void DrawStone(uint8_t house_id);
        void MoveStoneToTarget(uint8_t start_point);
        // void DisableLastCoil(int logic_coli_id);
        // void EnableSingleCoil(int logic_coil_id, bool enable_it);
        // void DisableAllCoils(void);
        // void Scanner(void);
        // void Test(uint8_t chip_index);
        // void Test0(void);test_h_bridge
        // void test_h_bridge(void);
        // void set_h_coil(int coil_index,int state);

        

    protected:
    private:
        House();
        // House(House const& copy);            // Not Implemented
        // House& operator=(House const& copy); // Not Implemented
        RobotAction* __house_action;
        
        motor_position ik(int x, int y) override;

        // Servo __LeftServo;
        // Servo __RightServo;
        // mcp23018* __Mcp23018[CHIPS_COUNT];
        // // void __EnableSingleCoil(int phisical_pin_id, bool enable_it);
        // uint8_t __current_coil_index;

        // uint8_t __I2cAddress[CHIPS_COUNT];   //Index is logic coil id, value is phical coil id
        // uint8_t __Chip_Index[COIL_COUNT];
        // uint8_t __Pin_in_chip[COIL_COUNT];
        // uint8_t __NextCoilId[COIL_COUNT];
        // uint8_t __UINT8_ZERO = 0;
        // uint8_t __CHIPS_COUNT;
        // uint8_t last_enabled_logic_coil_id;


};