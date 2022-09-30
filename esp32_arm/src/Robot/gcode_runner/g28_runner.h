#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "CNC/mover/mover_base.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "sync_gcode_runner_base.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        void Start() override ;
        bool IsDone() override;

    protected:
        virtual void SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb);
        virtual void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb);
        
    private:
        // void __HomeSingleAxis(EnumAxis axis);
        // void _running_G28();

        AxisHomer* __homer;
        MoverBase* __mover;
        HomingConfig* __config;
};

