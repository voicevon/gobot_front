#pragma once

#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "MyLibs/app_base/cnc_app_base.h"


class BoxCarrierApp: public CncAppBase{
    public:
        enum BoxMoverState{ START, 
                                RESETTING, 
                                READY_TO_LOAD, 
                                LOADING, 
                                LOADED, 
                                VERTICAL_UP, 
                                READY_TO_UNLOAD,
                                UNLOADING, 
                                UNLOADED
                                };
        BoxCarrierApp::BoxMoverState State;

        BoxCarrierApp();
        void ParkArms(bool do_homing);
        void LoadBox();
        void UnloadBox();
        void SpinOnce(); 
        // void onGot_MqttMessage(const char*) override;
        BoxCarrierApp::BoxMoverState GetState(){return this->State;};
    
    private:
   
};