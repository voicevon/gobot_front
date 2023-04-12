#pragma once

#include "MyLibs/app_base/cnc_app_base.h"

class SpringMakerApp: public CncAppBase{
    public:
        SpringMakerApp();
        // void SpinOnce(); 
        void __onGot_MqttMessage(const char*);
        
    private:
   
};