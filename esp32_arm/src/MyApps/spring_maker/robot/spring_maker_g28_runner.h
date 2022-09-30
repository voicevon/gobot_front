#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/spring_maker/board/board.h"


class SpringMaker_G28_Runner: public G28_Runner{
    public:

      public:
        void Init(Board_SpringMaker* board, MoverBase* mover);

    private:
        void SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb) override;
        void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
        virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

        AxisHomer __homer;
        HomingConfig homing;
};