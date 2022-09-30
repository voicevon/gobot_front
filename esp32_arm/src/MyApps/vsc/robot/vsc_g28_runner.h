#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/vsc/board/board_vsc.h"
class Vsc_G28_Runner: public G28_Runner{
    public:
        void Init(Vsc_Board* board, MoverBase* mover);

    private:
        void SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb) override;
        void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
        virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

        AxisHomer __homer;
        HomingConfig homing;

};