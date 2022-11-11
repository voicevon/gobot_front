#pragma once

#include "Robot/robot_base.h"
#include "MyApps/teeth_wh/board/teeth_wh_board.h"
#include "CNC/arm_solution/core_xy_xa_ab/core_xy_xa_ab_arm_solution.h"
#include "teeth_wh_g28_runner.h"
#include "MyApps/teeth_wh/board/eef/teeth_wh_eef.h"


#define ROWS_COUNT 20
#define COLS_COUNT 8
#define CNC_PLANNER_BLOCK_COUNT 88
#define CNC_ACTUATORS_COUNT 2

class TeechWarehouse_Robot: public RobotBase{
    public:
        void Init(TeethWarehouse_Board* board);
        void StoreToCell(int row_index, int col_index);
        void RetrieveFromCell(int row_index, int col_index);

    private:
        void __Init_actuators(TeethWarehouse_Board* board);

        CncActuatorBase* __all_actuators[CNC_ACTUATORS_COUNT];
	    // CncActuatorStepper __actuator_alpha;
	    // CncActuatorStepper __actuator_beta;
        CncActuatorFastStepper __actuator_alpha;
        CncActuatorFastStepper __actuator_beta;
        
        CncMoverBase mover;
        CncSolution_CoreXY_XA_ab arm_solution;
        TeethWarehouse_G28_Runner g28_runner;

        Core_XY_XA_ab_config arm_config;
        MoveBlock __all_move_blocks[CNC_PLANNER_BLOCK_COUNT]; 
        LineSegment __all_line_segments[88];

        TeethWarehouse_RobotEef __eef;

        TeethWarehouse_Board* __board;
        //Eef actions:
        void __MoveToCell(int row_index, int col_index);
        void __EefMoveToCollectionBox();
        void __EefMoveToTop();

        void __TurnOn_VacuumePump(bool is_turn_on){this->__board->EnableVacuumPump(is_turn_on);};

        float __GetWeight();
        void __GetDistanceToTeeth();

        void __Open_BottomCover(bool is_open);


};