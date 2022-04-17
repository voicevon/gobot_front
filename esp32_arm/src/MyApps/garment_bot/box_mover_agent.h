#pragma once


class GarmentBoxMoverAgent{
    public:
        // TODO: This declaration shold be unified for:  BoxMover, and BoxMoverAgent
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
        void SpinOnce(){};
        void PresetState(BoxMoverState the_state){this->__preset_state = the_state;};
        void ToPresetState(){this->ToState(this->__preset_state);};
        
    private:
        BoxMoverState __preset_state;
        void ToState(BoxMoverState state);
        void LoadBox(){};
        void UnloadBox(){};
};