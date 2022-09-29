#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "sync_gcode_runner_base.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        // void LinkGcode(Gcode* gcode) override;
        void Start() override ;
        bool IsDone() override;
        
    private:
        // EnumAxis __homing_axis;
        // Gcode* __gcode;
        void __HomeSingleAxis(EnumAxis axis);
        void _running_G28();

        // Homer* __homer;
        // MoverBase* __mover;
        // HomeConfig* __config;


};