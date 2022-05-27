#pragma once

// #include "box_carrier_hw.h"
#include "Robot/cnc_core_yz/cnc_core_yz.h"
#include "Robot/gcode_queue.h"
#include "Robot/gcode_producer.h"
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
        void ExecuteMqttCommand(const char*) override;
        BoxCarrier::BoxMoverState GetState(){return this->State;};
    
    private:
   
};