#pragma once

// #include "CNC/arm_solution/core_az/box_mover_hw.h"
#include "CNC/arm_solution/core_az/cnc_core_az.h"
#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"


class BoxMover: public GcodeProducer, public MqttMessageConsumer{
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
        BoxMover::BoxMoverState State;

        BoxMover();
        void ParkArms(bool do_homing);
        void LoadBox();
        void UnloadBox();
        void SpinOnce(); 
        void AsyncExecuteMqttCommand(const char*) override;
        BoxMover::BoxMoverState GetState(){return this->State;};
    
    private:
   
};