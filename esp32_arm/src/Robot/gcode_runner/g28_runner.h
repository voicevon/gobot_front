#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/mover/cnc_mover_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "sync_gcode_runner_base.h"
#include "Robot/axis_homer/position_trigger_array.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        virtual void Init(CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position);
    
        void Start() override ;
        bool IsDone() override;
        void LinkGcode(GcodeText* gcode) override;
        
    protected:
        virtual void _InitHomePosition(char axis_name, float setting_position);
        virtual void _SetMoveBlock_ToHome(char axis_name, MoveBlock* mb);
        virtual void _SetHomedPosition(PositionTrigger* firer);
        CncMover* _mover;   //Only for stop()
        ArmSolutionBase* _arm_solution;  //for Setcurrent_position
        char _axis_name;
        bool _has_position_trigger = true;   //TODO: to be:    blank_arm_solution.

    private:

};

