#pragma once

#include "box_mover_hw.h"

class BoxMover{


    public:
        enum EnumState{ START, 
                                RESETTING, 
                                READY_TO_LOAD, 
                                LOADING, 
                                LOADED, 
                                VERTICAL_UP, \
                                READY_TO_UNLOAD,
                                UNLOADING, 
                                UNLOADED
                                };
        EnumState State;
        void ResetToLoad();
        void LoadBox();
        void UnloadBox();
        void SpinOnce();        
};