#include "Robot/gcode_runner/g28_runner.h"
// #include "MyApps/asar_agv/board/sub_board_cnc2205/board_cnc_2205.h"
#include "MyApps/warehouse/asar_agv/board/sub_board_cnc2205/board_cnc_2205.h"

class BoxMover_G28_Runner: public G28_Runner{
    public:
        void Init(CncMover* mover);

    private:
        void _SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
        // void _SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
        // virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        // virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

        // AxisHomer __homer;
        // HomingConfig homing;

};