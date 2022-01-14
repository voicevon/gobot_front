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


#define PIN_LED_POWER_2109 12
#define PIN_LED_B_2109 25
#define PIN_LED_C_2109 26
#define PIN_LED_F_2019 27
#define PIN_LED_H_2109 14

#define PIN_HOME_ALHPA_2109 35     //??
#define PIN_ALPHA_DIR_2109 19
#define PIN_ALPHA_STEP_2109 5

#define PIN_HOME_BETA_2109 34      //??
#define PIN_BETA_DIR_2109 17
#define PIN_BETA_STEP_2109 4

#define ENDER_COIL_2109 32
#define ENDER_COIL_EXT_2109 33


class GobotHouse: public RobotBase{
    public:
        static GobotHouse& getInstance()
        {
            static GobotHouse instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        // void SpinOnce(void);
        void Init_Linkage() override;
        void Setup(RobotAction* pAction, int segments);
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;
        void MoveStone_FromRoomToHead(uint8_t house_id);
        void MoveStone_FromHeadToRoom(uint8_t house_id);



    private:
        GobotHouse();
        // ik_position ik(float x, float y) override;
        void ActuatorMoveTo_FK(float x, float y);
        void ActuatorMoveTo_IK(int32_t a, int32_t b);
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
        virtual IkPositionBase* IK(FkPositionBase* fk) override;
        virtual FkPositionBase* FK(IkPositionBase* ik) override;
        RobotAction* __house_action;
        int __segments;
        
        void __Move_fromRoom_toDoor(uint8_t house_id, bool forwarding);
        void __Move_fromHead_toNeck(bool forwarding);
        void __Move_fromNeck_toDoor(uint8_t house_id, bool forwarding);
        void __Enable_eefCoil(bool enable);
        void RunG6(Gcode* gcode) override {};   //Block mode
        void _running_G1() override{};
        void _running_G28() override;
        std::string GetHomeTrigerStateString() override {};
        void MoveToTargetPosition() {};

        HouseMap __map;

        // SingleAxis objAxis_Alpha= SingleAxis();
        // SingleAxis objAxis_Beta= SingleAxis();
        ActuatorBase objActuator_Alpha = ActuatorBase();
        ActuatorBase objActuator_Beta = ActuatorBase();


        Led objLedPower = Led(0, PIN_LED_POWER_2109, LOW);
        Led objLedHomeAlpha = Led(1,2,LOW);
        HomeHelper objHomeHelper_alpha = HomeHelper(PIN_HOME_ALHPA_2109, HIGH);
        HomeHelper objHomeHelper_beta = HomeHelper(PIN_HOME_BETA_2109, HIGH);

        Stepper objStepper_alpha = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        Stepper objStepper_beta = Stepper(PIN_BETA_STEP_2109, PIN_BETA_DIR_2109);
        StepControl objStepControl;

        CommuUart objCommuUart = CommuUart();
        CommuBleGattServer objCommuBle = CommuBleGattServer();
        FkPosXY objFkXY;
        IkPosXY objIkXY;

        void init_gpio();

        Stepper* __homing_stepper;
        HomeHelper* __homing_helper;
};