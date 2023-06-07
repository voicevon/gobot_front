#pragma once

#include "MyLibs/app_base/cnc_base/cnc_app_base.h"
#include "robot/sillicon_pump_robot.h"

class Silicon_Pump_App: public CncAppBase{
    public:
        Silicon_Pump_App();
        void LinkRobot(Silicon_Pump_Robot* robot){this->__robot=robot;};
        // void onGot_MqttMessage(const char*) override;
        // void ExecuteAppCommand(const char* command) override{}; 
        
    private:
        // int __robot_row_id = 0;
        Silicon_Pump_Robot* __robot;

};