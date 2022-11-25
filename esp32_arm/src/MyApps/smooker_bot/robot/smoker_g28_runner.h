#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/smooker_bot/board/smoker_board.h"

class Smoker_G28_Runner: public G28_Runner{
    public:
        void Init(CncMover* mover) override;

    private:
        void SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
        // void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
        // virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        // virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

        // AxisHomer __homer;
        // HomingConfig homing;

};