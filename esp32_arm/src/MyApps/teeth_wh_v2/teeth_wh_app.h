#pragma once

#include "MyApps/app_base.h"
#include "robot/twh2_robot.h"

class Twh2_App: public AppBase{
    public:
        Twh2_App();
        void LinkRobot(Twh2_Robot* robot){this->__robot=robot;};
        void AsyncExecuteMqttCommand(const char*) override;
        
    private:

        Twh2_Robot* __robot;

};