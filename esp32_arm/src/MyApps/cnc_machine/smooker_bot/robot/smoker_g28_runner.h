#include "Robot/gcode_runner/g28_runner.h"
#include "MyApps/cnc_machine/smooker_bot/board/smoker_board.h"

class Smoker_G28_Runner: public G28_Runner{
    public:
        void Init(CncMover* mover, ArmSolutionBase* arm_solution, const char* mqtt_topic_for_home_position) override;

    private:
        void _SetMoveBlock_ToHome(char axis, MoveBlock* mb) override;
        void _SetHomedPosition(PositionTrigger* firer) override;
        // void _SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;
        // virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        // virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);

        // AxisHomer __homer;
        // HomingConfig homing;

};