#pragma once

#include "CNC/board/cnc_board_base.h"
#include "kinematic_config.h"
#include "../coordinate/coordinate_base.h"
#include "../coordinate/cnc_axis.h"
#include "MyLibs/pid_controllers/pid_controllers.h"
// #include "CNC/planner/planner.h"

#include "Robot/eef/eef_standard_code.h"
#include "CNC/gcode/gcode_consumer.h"
#include "../mover/mover_base.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"

enum class CncState{
    IDLE,
    RUNNING_G1,
    RUNNING_G4,
    RUNNING_G28
};

class ArmSolutionBase{
    public:
        CncState State = CncState::IDLE;
        void SpinOnce();

        void RunGcode(Gcode* gcode);  //todo:  rename to RunMovement(),  CncMoveTo()
        void RunG28(EnumAxis axis);
        virtual bool GetCurrentPosition(FkPositionBase* position_fk);

        // Todo:  should remove these two lines?
        virtual float GetDistanceToTarget_FK();
        virtual float GetDistanceToTarget_IK();


        void SayHello();    // TODO:  be virtual
        virtual void _SetCurrentPositionAsHome(EnumAxis homing_axis);
        virtual EnumAxis ConvertToEnum(char axis);
        void ForceStopMover();
        CncBoardBase* _cnc_board;   // TODO:  remove this.
        // Planner* planner;
        // MoverBase* _mover_base;   // Should this be here?
        Queue_MoveBlock* __queue_move_block;

        
    protected:
        void __CutLineSegment_ToMoveBlocks_to_queue(LineSegment* line);
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik);
        virtual void FK(IkPositionBase* from_ik,FkPositionBase* to_fk);
        virtual void RunG1(Gcode* gcode);   //None blocking, move backgroundly.
        virtual void RunG6(Gcode* gcode);   //Block mode
        void RunG4(Gcode* gcode);
        virtual std::string GetHomeTrigerStateString();

        void Run_M42_OutputGpio(uint8_t pin_number, uint8_t pin_value);
        void RunM84();
        // void _running_G1();
        void _running_G28();
		virtual void _RunG28_CombinedFk(EnumAxis axis){};

        EnumAxis _homing_axis;
        KinematicConfig _config_base;    //TODO:  rename to _kinamatic_config
        CncHomers _cnc_homer = CncHomers(CNC_AXIS_COUNT);

        bool is_absolute_position = true;
        
        // LineSegmentQueue* _line_segment_queue;


    private:
        int test_int;
        void _base_spin_once();
        void __running_G4();
        void __HomeSingleAxis(EnumAxis axis);
        long __g4_start_timestamp;
        int __g4_time_second;

};

