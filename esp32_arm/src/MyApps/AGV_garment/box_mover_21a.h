#pragma once


#include "Robot/Actuator/DCMotor/h_bridge_l298n.h"
#include "Robot/Actuator/dual_end_actuator.h"

#define PIN_HOME_SENSOR_2130 32

#define PIN_DC_MOTOR_A_2130 27
#define PIN_DC_MOTOR_B_2130 14
#define PIN_LED_POWER_2130 28
#include "all_devices.h"

/*
    start --> resetting --> ready --> loading --> vertical_up  --> angle_down -->  unloading --> unloaded
*/
class BoxMover_21a{
    public:
        typedef enum EnumState{ START, 
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
        BoxMover_21a();
        void ResetToLoad();
        void LoadBox();
        void UnloadBox();
        void SpinOnce();

    protected:
        
    private:
        DualEndActuator* verticalMover;
        DualEndActuator* angleMover;
};
