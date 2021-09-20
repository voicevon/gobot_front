#pragma once

#include "actions.h"
#include <Arduino.h>
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/RobotArmBase.hpp"
#include "HouseMap.h"
#include "Robot/Commu/CommuBleGattServer.h"
// #define COIL_COUNT 53
// #define CHIPS_COUNT  4

#define HOUSE_ALPHA_AXIS 10
#define HOUSE_BETA_AXIS 11




class GobotHouse: public RobotArmBase<Stepper,StepControl,CommuBleGattServer>{
    public:
        static GobotHouse& getInstance()
        {
            static GobotHouse instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void SpinOnce(void);
        void Setup(RobotAction* pAction, int segments);
        void HomeAllAxises() override;
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
};