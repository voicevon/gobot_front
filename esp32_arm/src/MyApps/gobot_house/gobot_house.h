#pragma once

#include "actions.h"
#include <Arduino.h>
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/RobotBase.h"
#include "HouseMap.h"
// #include "Robot/Commu/CommuBleGattServer.h"
#include "MyLibs/MyFunctions.hpp"
#include "Robot/Commu/CommuUart.h"
#include "Robot/Commu/CommuBleGattServer.h"

// #include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeHelper.h"
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
    protected:
    private:
        GobotHouse();
        RobotAction* __house_action;
        int __segments;
        
        ik_position ik(float x, float y) override;
        void __Move_fromRoom_toDoor(uint8_t house_id, bool forwarding);
        void __Move_fromHead_toNeck(bool forwarding);
        void __Move_fromNeck_toDoor(uint8_t house_id, bool forwarding);
        void __Enable_eefCoil(bool enable);
        HouseMap __map;

        SingleAxis objAxis_Alpha= SingleAxis();
        SingleAxis objAxis_Beta= SingleAxis();
        ActuatorBase objActuator_Alpha = ActuatorBase();
        ActuatorBase objActuator_Beta = ActuatorBase();


        Led objLedPower = Led(0, PIN_LED_POWER, LOW);
        Led objLedHomeAlpha = Led(1,2,LOW);
        HomeHelper objHomeTriger_alpha = HomeHelper(PIN_HOME_ALHPA, HIGH);
        HomeHelper objHomeTriger_beta = HomeHelper(PIN_HOME_BETA, HIGH);

        Stepper objStepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
        Stepper objStepper_beta = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
        StepControl objStepControl;

        CommuUart objCommuUart = CommuUart();
        CommuBleGattServer objCommuBle = CommuBleGattServer();



        void init_gpio();
};