#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/mover/mover_base.h"
// #include "Robot/axis_homer/axis_homer.h"
#include "sync_gcode_runner_base.h"
#include "Robot/axis_homer/home_trigger_array.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        virtual void Init(MoverBase* mover);
    
        void Start() override ;
        bool IsDone() override;
        void LinkGcode(Gcode* gcode) override;
        float GetTriggerPosition(){return __last_homed_position;};

    protected:
        virtual void SetMoveBlock_ToHome(char axis_name, MoveBlock* mb);
        MoverBase* __mover;   //Only for stop()
        
    private:
        char __axis_name;
        float __last_homed_position;
};

