#pragma once

#include "actions.h"
#include <Arduino.h>
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/RobotBase.h"
#include "HouseMap.h"
// #include "Robot/Commu/CommuBleGattServer.h"
#include "MyLibs/MyFunctions.hpp"

// #include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeTriger.h"
#include "MyLibs/Components/Led.h"

// #define COIL_COUNT 53
// #define CHIPS_COUNT  4

#define HOUSE_ALPHA_AXIS 10
#define HOUSE_BETA_AXIS 11


#define PIN_LED_POWER 12
#define PIN_LED_B 25
#define PIN_LED_C 26
#define PIN_LED_F 27
#define PIN_LED_H 14

#define PIN_HOME_ALHPA 35     //??
#define PIN_ALPHA_DIR 19
#define PIN_ALPHA_STEP 5
#define PIN_ALPHA_ENABLE 18

#define PIN_HOME_BETA 34      //??
#define PIN_BETA_DIR 17
#define PIN_BETA_STEP 4
#define PIN_BETA_ENABLE 16

#define ENDER_COIL 32
#define ENDER_COIL_EXT 33

#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23

class GobotHouse: public RobotBase{
    public:
        static GobotHouse& getInstance()
        {
            static GobotHouse instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void SpinOnce(void);
        void Init() override;
        void Setup(RobotAction* pAction, int segments);
        void HomeAllAxises() override;
        void RunG1(Gcode* gcode) override;
        void MoveStone_FromRoomToHead(uint8_t house_id);
        void MoveStone_FromHeadToRoom(uint8_t house_id);
        // StepControl steppers;   //???
    protected:
    private:
        GobotHouse();
        // SingleAxisBase<Stepper> obj_axis_alpha = SingleAxisBase<Stepper>('A');
        // SingleAxisBase<Stepper> obj_axis_beta = SingleAxisBase<Stepper>('B');

        RobotAction* __house_action;
        int __segments;
        
        ik_position ik(float x, float y) override;
        // void __HomeSpin(Stepper* homing_stepper, uint8_t home_pin);
        void __Move_fromRoom_toDoor(uint8_t house_id, bool forwarding);
        void __Move_fromHead_toNeck(bool forwarding);
        void __Move_fromNeck_toDoor(uint8_t house_id, bool forwarding);
        void __Enable_eefCoil(bool enable);
        HouseMap __map;

        Led led_power = Led(0, PIN_LED_POWER, LOW);
        // led_Robot = Led(2,1,LOW);
        Led led_home_alpha = Led(1,2,LOW);
        HomeTriger homeTriger_alpha = HomeTriger(PIN_HOME_ALHPA, HIGH);
        HomeTriger homeTriger_beta = HomeTriger(PIN_HOME_BETA, HIGH);

        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
        StepControl stepControl;
};