#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/spring_maker/board/board.h"

#define POSITION_TRIGGER_COUNT 1

class SpringMaker_G28_Runner: public G28_Runner{
    public:

      public:
        void Init(Board_SpringMaker* board, MoverBase* mover);

    private:
        void SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
//         void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
//         virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
//         virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

//         // AxisHomer __homer;
//         PositionTrigger __all_position_triggers[POSITION_TRIGGER_COUNT];
//         HomingConfig homing;
};