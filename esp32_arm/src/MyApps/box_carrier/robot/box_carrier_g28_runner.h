#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/asar_agv/board/sub_board_cnc2206/board_cnc_2206.h"

class BoxCarrier_G28_Runner: public G28_Runner{
    public:
        void Init(Board2204Cnc* board, MoverBase* mover);

    private:
        void SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
        // void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
        // virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        // virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

        // AxisHomer __homer;
        // HomingConfig homing;

};