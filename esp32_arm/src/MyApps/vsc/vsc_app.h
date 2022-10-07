#pragma once

#include "MyApps/app_base.h"
#include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"
#define LAYER_COUNT 20

class VscApp: public AppBase{
    public:
        VscApp();
        void AsyncExecuteMqttCommand(const char*) override;
        void MoveTo(int layer_index, int cell_index);
        
    private:
        CncActuatorDcMotor* __motor;
        int __position_in_pitch[LAYER_COUNT];
        int __GEAR_TEETH = 56;
        int __PITCH_COUNT = 157;

};