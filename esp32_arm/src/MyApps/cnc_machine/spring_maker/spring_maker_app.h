#pragma once

#include "MyApps/app_base.h"

class SpringMakerApp: public AppBase{
    public:
        SpringMakerApp();
        // void SpinOnce(); 
        void onGot_MqttMessage(const char*) override;
        
    private:
   
};