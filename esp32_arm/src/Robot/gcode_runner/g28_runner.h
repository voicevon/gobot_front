#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "sync_gcode_runner_base.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        EnumAxis HomingAxis;
        void Start() override {};
        bool IsDone() override{};
        
    private:


};