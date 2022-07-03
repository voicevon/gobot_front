#pragma once

// #include "CNC/solution/core_yz/cnc_core_yz_base.h"
#include "cnc/solution.h"
#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"


class BoxCarrier: public GcodeProducer, public MqttMessageConsumer{
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
        BoxCarrier::BoxMoverState State;

        BoxCarrier();
        void ParkArms(bool do_homing);
        void LoadBox();
        void UnloadBox();
        void SpinOnce(); 
        void AsyncExecuteMqttCommand(const char*) override;
        BoxCarrier::BoxMoverState GetState(){return this->State;};
    
    private:
   
};