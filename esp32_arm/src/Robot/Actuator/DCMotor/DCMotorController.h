#pragma once


#include "../ActuatorControllerBase.h"
#include "Robot/Actuator/DCMotor/DcMotor.h"

class DCMotorController: public ActuatorControllerBase<DCMotor>{
    public:
        void MoveForward();
        void MoveBackward();
        
    private:

};