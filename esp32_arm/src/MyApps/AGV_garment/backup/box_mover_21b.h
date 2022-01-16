#pragma once
#include "all_devices.h"


// #include "Robot/Actuator/DCMotor/h_bridge_l298n.h"
// #include "Robot/Actuator/dual_end_actuator.h"

// #define PIN_HOME_SENSOR_2130 32

// #define PIN_DC_MOTOR_A_2130 27
// #define PIN_DC_MOTOR_B_2130 14
// #define PIN_LED_POWER_2130 28
#include "Robot/RobotBase.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/Actuator/ActuatorBase.h"

#define PIN_STEP_ALPHA 22
#define PIN_DIR_ALPHA 22
#define PIN_STEP_BETA 22
#define PIN_DIR_BETA 22
/*
    start --> resetting --> ready --> loading --> vertical_up  --> angle_down -->  unloading --> unloaded
*/
class BoxMover_21b:public RobotBase{
    public:
        enum EnumState{ START, 
                                RESETTING, 
                                READY_TO_LOAD, 
                                LOADING, 
                                LOADED, 
                                VERTICAL_UP, \
                                READY_TO_UNLOAD,
                                UNLOADING, 
                                UNLOADED
                                };
        EnumState State;
        BoxMover_21b(){};
        
        // void Init_VerticalMover(DualEndActuator* actuator, uint8_t pin_endstop_north, uint8_t pin_endstop_south, DriverBase* driver);
        // void Init_AngleMover(DualEndActuator* actuator, uint8_t pin_endstop_north, uint8_t pin_endstop_south, DriverBase* driver);

        void ResetToLoad();
        void LoadBox();
        void UnloadBox();
        void SpinOnce();
        // DualEndActuator* angleMover;
        // DualEndActuator* verticalMover;



    protected:
        
    private:
        ActuatorBase objActuator = ActuatorBase();
        Stepper objStepper_alpha = Stepper(PIN_STEP_ALPHA, PIN_DIR_ALPHA);
        Stepper objStepper_beta = Stepper(PIN_STEP_BETA, PIN_DIR_BETA);
        StepControl objStepControl;
};
