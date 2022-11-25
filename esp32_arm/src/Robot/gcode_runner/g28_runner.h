#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/mover/cnc_mover_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "sync_gcode_runner_base.h"
#include "Robot/axis_homer/home_trigger_array.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        virtual void Init(CncMover* mover, ArmSolutionBase* arm_solution);
    
        void Start() override ;
        bool IsDone() override;
        void LinkGcode(Gcode* gcode) override;
        
    protected:
        virtual void SetMoveBlock_ToHome(char axis_name, MoveBlock* mb);
        virtual void SetHomedPosition(PositionTrigger* firer);
        CncMover* _mover;   //Only for stop()
        ArmSolutionBase* _arm_solution;  //for Setcurrent_position
        char _axis_name;

    private:
};

