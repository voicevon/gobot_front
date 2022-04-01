#pragma once

#include "box_mover_hw.h"
#include "Robot/gcode_queue.h"
#include "Robot/gcode_producer.h"

class BoxMover: public GcodeProducer{
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
        BoxMover::BoxMoverState GetState(){return this->State;};
        // GcodeQueue* GetCommandQueue(){return this->__commandQueue;};
    
    private:
        // GcodeQueue* __commandQueue;
        // BoxMoverHardware* __objBoxMoverHardware;
   
};