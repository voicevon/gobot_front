#pragma once

#include "MyApps/app_base.h"
#include "robot/loop_porter_robot.h"

class Twh_LoopPorter_App: public AppBase{
    public:
        Twh_LoopPorter_App(int row_id);
        void LinkRobot(Twh_LoopPorter_Robot* robot){this->__robot=robot;};
        void onGot_MqttMessage(const char*) override;
        
    private:
        int __robot_row_id = 0;
        Twh_LoopPorter_Robot* __robot;

};