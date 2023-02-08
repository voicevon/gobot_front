#pragma once

#include "MyApps/app_base.h"
#include "robot/packer_robot.h"

class Twh_Packer_App: public AppBase{
    public:
        Twh_Packer_App();
        void LinkRobot(Twh_Packer_Robot* robot){this->__robot=robot;};
        void onGot_MqttMessage(const char*) override;
        
    private:
        Twh_Packer_Robot* __robot;

};