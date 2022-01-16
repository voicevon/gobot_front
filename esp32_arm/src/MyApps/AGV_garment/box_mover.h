#pragma once

#include "box_mover_hw.h"
#include "Robot/command_queue.h"

class BoxMover{
    public:
        enum EnumState{ START, 
                                RESETTING, 
                                READY_TO_LOAD, 
                                LOADING, 
                                LOADED, 
                                VERTICAL_UP, 
                                READY_TO_UNLOAD,
                                UNLOADING, 
                                UNLOADED
                                };
        EnumState State;

        BoxMover();
        void ResetToLoad();
        void LoadBox();
        void UnloadBox();
        void SpinOnce();  
    
    private:
        CommandQueue* __commandQueue;
        BoxMoverHardware* __robot_hardware;
   
};