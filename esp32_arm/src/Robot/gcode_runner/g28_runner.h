#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/mover/cnc_mover_base.h"
#include "sync_gcode_runner_base.h"
#include "Robot/axis_homer/home_trigger_array.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        virtual void Init(CncMover* mover);
    
        void Start() override ;
        bool IsDone() override;
        void LinkGcode(Gcode* gcode) override;
        // float GetTriggerPosition(){return __last_homed_position;};
        // char GetHomingAxisName(){return this->__axis_name;};
        
    protected:
        virtual void SetMoveBlock_ToHome(char axis_name, MoveBlock* mb);
        virtual void SetHomedPosition(float triggered_position);
        CncMover* _mover;   //Only for stop()
        char _axis_name;

        // float __last_homed_position;
    private:
};

