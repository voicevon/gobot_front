#pragma once


#include "../ActuatorControllerBase.h"
#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "DCDriverHBridge.h"

class DCMotorController: public ActuatorControllerBase{
    public:
        void MoveForward(float speed, bool reverseDir) override;  // For DC Motor.
        void LinkDriver(DCDriverHBridge* hBridge){this->hBridge=hBridge;};

    private:
        DCDriverHBridge* hBridge;
};