#pragma once

// #include "MyApps/app_base.h"
#include "MyLibs/app_base/cnc_app_base.h"
#include "robot/vsc_robot.h"
// #include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"
// #define LAYER_COUNT 20

class VscApp: public CncAppBase{
    public:
        VscApp();
        void LinkRobot(VscRobot* robot){this->__robot=robot;};
        void __onGot_MqttMessage(const char*);
        
    private:
        VscRobot* __robot;


};