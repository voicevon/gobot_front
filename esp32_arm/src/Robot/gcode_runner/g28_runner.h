#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "Robot/axis_homer/axis_homer.h"
// #include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "CNC/mover/mover_base.h"
// #include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "sync_gcode_runner_base.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        void Start() override ;
        bool IsDone() override;
        // void LinkMover(MoverBase* mover){this->__mover=mover;};
        void LinkHomer(AxisHomer* homer){this->__homer=homer;};
        void LinkGcode(Gcode* gcode) override;

    protected:
        virtual void SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb);
        virtual void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb);
        virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);
        MoverBase* __mover;
        AxisHomer* __homer;
        
    private:
        char __axis_name;
        bool home_actuator_directly;
};

