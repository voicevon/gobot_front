#pragma once

#include "MyApps/app_base.h"
#include "robot/twh_robot.h"

class Twh2_App: public AppBase{
    public:
        Twh2_App();
        void LinkRobot(Twh2_Robot* robot){this->__robot=robot;};
        void AsyncExecuteMqttCommand(const char*) override;
        
    private:

        // Twh2_XzArmSolution_Config* __robot;
        Twh2_Robot* __robot;
        // CncActuatorDcMotor* __motor;
        // int __position_in_pitch[LAYER_COUNT];
        // int __GEAR_TEETH = 56;
        // int __PITCH_COUNT = 157;

};