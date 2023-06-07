#pragma once

#include "MyLibs/app_base/cnc_base/cnc_app_base.h"
#include "robot/vsc_robot.h"

class VscApp: public CncAppBase{
    public:
        VscApp();
        void LinkRobot(VscRobot* robot){this->__robot=robot;};
        void __onGot_MqttMessage(const char*);
        // void ExecuteAppCommand(const char* command) override{}; 
        
    private:
        VscRobot* __robot;


};