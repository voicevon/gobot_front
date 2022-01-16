#pragma once

#include "Robot/RobotBase.h"
#include "ESP32Step/src/TeensyStep.h"
#include "box_mover_hw.h"


#define PIN_STEP_ALPHA 22
#define PIN_DIR_ALPHA 22
#define PIN_STEP_BETA 22
#define PIN_DIR_BETA 22


class BoxMoverHardware{


    private:
        // ActuatorBase objActuator = ActuatorBase();
        Stepper objStepper_alpha = Stepper(PIN_STEP_ALPHA, PIN_DIR_ALPHA);
        Stepper objStepper_beta = Stepper(PIN_STEP_BETA, PIN_DIR_BETA);
        StepControl objStepControl;
};