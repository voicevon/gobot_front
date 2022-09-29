#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "sync_gcode_runner_base.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        void Start() override ;
        bool IsDone() override;
        
    private:
        void __HomeSingleAxis(EnumAxis axis);
        void _running_G28();

        // Homer* __homer;
        // MoverBase* __mover;
        // HomeConfig* __config;


};