#pragma once

#include "MyLibs/app_base/cnc_app_base.h"
#include "robot/loop_porter_robot.h"

class Twh_LoopPorter_App: public CncAppBase{
    public:
        Twh_LoopPorter_App(int row_id);
        void LinkRobot(Twh_LoopPorter_Robot* robot){this->__robot=robot;};
        void ExecuteCommand(const char* command) override; 
        
    private:
        int __robot_row_id = 0;
        Twh_LoopPorter_Robot* __robot;

};