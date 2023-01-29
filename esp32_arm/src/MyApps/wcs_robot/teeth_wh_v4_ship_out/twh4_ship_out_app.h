#pragma once

#include "MyApps/app_base.h"
#include "robot/twh4_ship_out_robot.h"

class Twh4_ShipOut_App: public AppBase{
    public:
        Twh4_ShipOut_App(int row_id);
        void LinkRobot(Twh4_ShipOut_Robot* robot){this->__robot=robot;};
        void onGot_MqttMessage(const char*) override;
        
    private:
        int __robot_row_id = 0;
        Twh4_ShipOut_Robot* __robot;

};