#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/vsc/board/board_vsc.h"
#include "Robot/axis_homer/homer_diction.h"
#define HOMERS_COUNT 1

class Vsc_G28_Runner: public G28_Runner{
    public:
        void Init(Vsc_Board* board, MoverBase* mover);

    private:
        void SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb) override;
        void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
        // virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        // virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

        // HomerDiction __homer_diction;
        // AxisHomer __homer;
        HomingConfig homing;
        // AxisHomer homers[HOMERS_COUNT];




};