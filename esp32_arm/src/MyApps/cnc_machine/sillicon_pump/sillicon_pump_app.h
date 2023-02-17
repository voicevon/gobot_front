#pragma once

#include "MyApps/app_base.h"
#include "robot/sillicon_pump_robot.h"

class Sillicon_Pump_App: public AppBase{
    public:
        Sillicon_Pump_App();
        void LinkRobot(Sillicon_Pump_Robot* robot){this->__robot=robot;};
        void onGot_MqttMessage(const char*) override;
        
    private:
        // int __robot_row_id = 0;
        Sillicon_Pump_Robot* __robot;

};