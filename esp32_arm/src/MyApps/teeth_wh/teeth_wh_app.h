#pragma once

#include "MyApps/app_base.h"
#include "robot/teeth_wh_robot.h"

class TeethWarehouse_App: public AppBase{
    public:
        TeethWarehouse_App();
        void LinkRobot(TeechWarehouse_Robot* robot){this->__robot=robot;};
        void AsyncExecuteMqttCommand(const char*) override;
        
    private:

        TeechWarehouse_Robot* __robot;
        // CncActuatorDcMotor* __motor;
        // int __position_in_pitch[LAYER_COUNT];
        // int __GEAR_TEETH = 56;
        // int __PITCH_COUNT = 157;

};