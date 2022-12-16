#pragma once

#include "MyApps/app_base.h"
#include "robot/twh_robot.h"

class Twh2Row_App: public AppBase{
    public:
        Twh2Row_App(int row_id);
        void LinkRobot(Twh2Row_Robot* robot){this->__robot=robot;};
        void AsyncExecuteMqttCommand(const char*) override;
        
    private:
        int __row_id = 0;
        Twh2Row_Robot* __robot;

};