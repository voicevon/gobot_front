#pragma once


class GarmentBoxMoverAgent{
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
        BoxMoverState ReadState(){return START;};
        void ToState(BoxMoverState state){};
        void LoadBox(){};
        void UnloadBox(){};
        void SpinOnce(){};
};