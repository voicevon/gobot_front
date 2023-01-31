#pragma once

// #include "CNC/arm_solution/core_yz/cnc_core_yz_base.h"
// #include "cnc/solution.h"
#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "Mqtt/mqtt_message_consumer.h"


class BoxCarrierApp: public GcodeProducer, public MqttMessageConsumer{
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
        void onGot_MqttMessage(const char*) override;
        BoxCarrierApp::BoxMoverState GetState(){return this->State;};
    
    private:
   
};