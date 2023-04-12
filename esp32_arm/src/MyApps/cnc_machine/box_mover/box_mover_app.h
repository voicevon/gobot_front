#pragma once

#include "CNC/arm_solution/core_xa_ab/core_az_arm_solution.h"
#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "MyLibs/app_base/cnc_app_base.h"


class BoxMoverApp: public CncAppBase{
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
        BoxMoverApp::BoxMoverState State;

        BoxMoverApp();
        void ParkArms(bool do_homing);
        void LoadBox();
        void UnloadBox();
        void SpinOnce(); 
        // void onGot_MqttMessage(const char*) override;
        BoxMoverApp::BoxMoverState GetState(){return this->State;};
        void ExecuteCommand(const char* command) override{};
    
    private:
   
};